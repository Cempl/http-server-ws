/*******************************************************************************/
#include "SendRecv.h"
#include "LogFile.h"
#include "Response.h"
#include "ControlsDatabase.h"


/*******************************************************************************/
vector<string> my_gList;
mutex glist_mutex;
condition_variable gdata_cond;

bool queueClear = false;


/*******************************************************************************/
void SendRecv::websocket_handshake(SSL* inSSL, string key)
{
	string response;

	generate_key(key);

	response += "HTTP/1.1 101 Switching Protocols\r\n";
	response += "Upgrade: websocket\r\n";
	response += "Connection: Upgrade\r\n";
	response += ("Sec-WebSocket-Accept: " + key + "\r\n");
	response += "Sec-WebSocket-Version: 13\r\n\r\n";
	
	try
	{
		my_send(inSSL, response, false);
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
	}

	Thread_recv(inSSL);
}


/*******************************************************************************/
void SendRecv::generate_key(string& key)
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

	const u_char* u_data = reinterpret_cast<const unsigned char*>(data.c_str());
	u_char temp_value;
	string temp_str = string();

	auto index_first_mask = 0;
	auto index_first_data_byte = 0;
	auto index_first_trash = 0;

	int a = u_data[0];

	if(u_data[0] == 136)
	{
		delete[] mask;

		// close connection
		return 1;
	}

	if (u_data[0] == 130)
	{
		delete[] mask;

		// binary data
		return 2;
	}

	length_code = u_data[1] & 127;

	if(length_code <= 125)
	{
		index_first_mask = 2;

		for(int i =0; i < 4; ++i)
		{
			mask[i] = u_data[i+index_first_mask];
		}
	}
	else
		if(length_code == 126)
		{
			index_first_mask = 4;

			for(int i =0; i < 4; ++i)
			{
				mask[i] = u_data[i+index_first_mask];
			}
		}
		else
			if(length_code == 127)
			{
				index_first_mask = 10;

				for(int i =0; i < 4; ++i)
				{
					mask[i] = u_data[i+index_first_mask];
				}
			}
			else
			{
				delete[] mask;

				throw exception("Error in websocket content");
			}

	index_first_data_byte = index_first_mask + 4;

	for(i = index_first_data_byte; i < data_length; ++i)
	{
		temp_value = u_data[i] ^ mask[j % 4];
		temp_str.push_back(reinterpret_cast<char&>(temp_value));

		++j;
	}

	data.swap(temp_str);

	delete[] mask;

	// text data
	return 0;
}


/*******************************************************************************/
void SendRecv::websocket_set_content(string& data, int64_t data_length, int data_type)
{
	string message = string();

	if (data_type == 2)
	{
		// binary data
		message = (char)130;
	}
	else
	{
		message = (char)129;
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
void SendRecv::send_data(SSL* inSSL)
{		
	int index = 0;
	int max_length = 100;
	bool need_clear = false;

	while(true)
	{		
		unique_lock<mutex> u_mutex(glist_mutex);

		try
		{
			if( queueClear && need_clear )
			{
				index = 0;
				need_clear = false;
			}

			interruptible_wait(gdata_cond, u_mutex, [&index]{return (index < my_gList.size());});
			my_send(inSSL, my_gList[index], false);
			++index;

			// Max length of the queue
			if( index == max_length )
			{
				my_gList.erase(my_gList.begin(), my_gList.begin() + max_length);
				queueClear = true;
				need_clear = true;
			}

			u_mutex.unlock();
		}
		catch(exception& e)
		{
			u_mutex.unlock();

			LogFile log;
			log.write(e.what());
		}
	}
}


/*******************************************************************************/
int SendRecv::recv_data(string& data, SSL* inSSL)
{
	int res_code = int();

	// Decode data
	auto result = websocket_get_content(data, static_cast<int>(data.size()));

	if(result == 1)
	{
		return 1;
	}
	
	{
		Response mResponse;
		res_code = mResponse.ParseDataFromWebSocket(data);
	}

	switch (res_code)
	{
		case 1:
		{
			// recoded data
			websocket_set_content(data, data.size(), result);

			lock_guard<mutex> guard(glist_mutex);

			// Added data in queue
			my_gList.push_back(data);

			// Wake up streams to send a data
			gdata_cond.notify_all();

			break;
		}

		case 0:
		case 2:
		{
			// recoded data
			websocket_set_content(data, data.size(), result);

			// send data
			my_send(inSSL, data, false);

			break;
		}

		case 3:
		{
			break;
		}

		case 4:
		{
			break;
		}

		case 5:
		{
			break;
		}

		default:
		{
			break;
		}
	}

	//// recoded data
	//websocket_set_content(data, data.size(), result);

	//lock_guard<mutex> guard(glist_mutex); 

	//// Added data in queue
	//my_gList.push_back(data);

	//// Wake up streams to send a data
	//gdata_cond.notify_all();
	
	return 0;
}


/*******************************************************************************/
int SendRecv::Thread_recv(SSL* inSSL)
{
	interruptible_thread thr(this, &SendRecv::send_data, inSSL);

	string message;

	while (true)
	{
		try
		{
			// Accept data from client
			my_recv(inSSL, message);

			int result = recv_data(message, inSSL);

			if(result == 1)
			{		
				thr.interrupt();
				thr.join();
				message.clear();

				break;
			}

			message.clear();
		}
		catch(exception& e)
		{
			message.clear();

			LogFile log;
			log.write(e.what());

			thr.interrupt();
			thr.join();
			message.clear();

			break;
		}
	}  

	return 0;
}