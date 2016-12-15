/*******************************************************************************/
#include "SendRecv.h"
#include "LogFile.h"


/*******************************************************************************/
vector<string> my_gList;
mutex glist_mutex;
condition_variable gdata_cond;



/*******************************************************************************/
void SendRecv::websocket_handshake(SOCKET client_socket, string key)
{
	string response;

	generate(key);

	response += "HTTP/1.1 101 Switching Protocols\r\n";
	response += "Upgrade: websocket\r\n";
	response += "Connection: Upgrade\r\n";
	response += ("Sec-WebSocket-Accept: " + key + "\r\n");
	response += "Sec-WebSocket-Version: 13\r\n\r\n";

	try
	{
		my_send(client_socket, response);
	}
	catch(OtherExceptions e)
	{
		LogFile log;
		log.write(e.what());
	}

	Thread_recv(client_socket);
}


/*******************************************************************************/
void SendRecv::generate(string& key)
{
	unsigned char hash[SHA_DIGEST_LENGTH];

	key += guid;
	
	SHA1((const unsigned char*)key.c_str(), strlen((const char*)key.c_str()), hash);

	key = (const char*)hash;

	if (key.size() > 20)
	{
		key = key.erase(20, key.length());

		key = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), static_cast<unsigned int>(key.length()));
	}
	else
	{
		key = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), static_cast<unsigned int>(key.length()));
	}
}


/*******************************************************************************/
int SendRecv::websocket_get_content(string& data, int data_length)
{
	// Mask length 4 bytes
	u_char* mask = new u_char [4];

	auto i = 0;
	auto j = 0;
	auto length_code = 0;

	u_char check_value;
	string temp_str = string();

	auto index_first_mask = 0;
	auto index_first_data_byte = 0;
	auto index_first_trash = 0;

	if((u_char)data[0] == 136)
	{
		delete[] mask;

		return 1; // close connection
	}

	if ((u_char)data[0] == 130)
	{
		delete[] mask;

		return 2; // binary data
	}

	length_code = (u_char) data[1] & 127;

	if(length_code <= 125)
	{
		index_first_mask = 2;

		for(int i =0; i < 4; ++i)
		{
			mask[i] = data[i+index_first_mask];
		}
	}
	else
		if(length_code == 126)
		{
			index_first_mask = 4;

			for(int i =0; i < 4; ++i)
			{
				mask[i] = data[i+index_first_mask];
			}
		}
		else
			if(length_code == 127)
			{
				index_first_mask = 10;

				for(int i =0; i < 4; ++i)
				{
					mask[i] = data[i+index_first_mask];
				}
			}
			else
			{
				delete[] mask;

				throw OtherExceptions("Error in websocket content");
			}

	index_first_data_byte = index_first_mask + 4;

	for(i = index_first_data_byte; i < data_length; ++i)
	{
		check_value = (u_char)data[i] ^ mask[j % 4];
		if(data[j] != check_value)
		{
			temp_str.push_back(check_value);
		}
		++j;
	}

	data.swap(temp_str);

	delete[] mask;

	return 0; // text data
}


/*******************************************************************************/
void SendRecv::websocket_set_content(string& data, int64_t data_length, int data_type)
{
	string message;

	if (data_type == 2)
	{
		message.push_back(130); // binary data
	}
	else
	{
		message.push_back(129);
	}

	if(data_length <= 125)
	{
		message.push_back(static_cast<char>(data_length));

		//data_start_index 2
	}
	else
		if(data_length > 125 && data_length <= 65535)
		{
			message.push_back(126);
			for(int i = 8; i >= 0; i = i-8)
				message.push_back(( data_length >> i ) & 255);
			
			//data_start_index 4
		}
		else
		{
			message.push_back(127);
			for(int i = 56; i >= 0; i = i-8)
				message.push_back(( data_length >> i ) & 255);

			//data_start_index 10
		}

	for(int i = 0; i < data_length; i++) 
	{
		message.push_back(data[i]);
	}

	data.swap(message);
}


/*******************************************************************************/
void SendRecv::send_data(SOCKET& client_socket)
{		
	int index = 0;

	while(true)
	{
		
			unique_lock<mutex> u_mutex(glist_mutex);

			try
			{
					interruptible_wait(gdata_cond, u_mutex, [&index]{return (index < my_gList.size());});
					my_send(client_socket, my_gList[index]);
					++index;

				u_mutex.unlock();
			}
			catch(OtherExceptions& e)
			{
				u_mutex.unlock();

				LogFile log;
				log.write(e.what());
			}
	}

}


/*******************************************************************************/
int SendRecv::recv_data(string& data)
{
	auto result = websocket_get_content(data, static_cast<int>(data.size())); // Раскодировали

	if(result == 1)
	{
		return 1;
	}

	websocket_set_content(data, data.size(), result); // Перекодировали сообщение

	lock_guard<mutex> guard(glist_mutex); 

	my_gList.push_back(data); // Записали сообщение в очередь
	gdata_cond.notify_all(); // Будим потоки для отправки сообщения
	
	return 0;
}


/*******************************************************************************/
int SendRecv::Thread_recv(SOCKET client_socket)
{
	interruptible_thread thr(this, &SendRecv::send_data,  client_socket);

	string message;

	while (true)
	{
		try
		{
			my_recv(client_socket, message); // Принимаем данные от клиента

			int result = recv_data(message);

			if(result == 1)
			{		
				thr.interrupt();
				thr.join();
				message.clear();

				return 0; // Normal exit
			}

			message.clear();
		}
		catch(OtherExceptions& e)
		{
			message.clear();

			LogFile log;
			log.write(e.what());
		}
	}  

	return 1; // Not normal exit
}