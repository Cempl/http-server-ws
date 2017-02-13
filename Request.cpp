/*******************************************************************************/
#include "ResponseRequest.h"
#include "LogFile.h"
#include "ControlsDatabase.h"
#include "Encryption.h"
#include "WSLexer.h"


/*******************************************************************************/
int ResponseRequest::Request(SSL* inSSL)
{
			bool				flag = bool();

			string				buf;	
			string				file_name = { 0 };
			string				extension_file = { 0 };
			string				not_supported_method = "method_is_not_supported.html";

			vector<string>		entire_query;
	const	vector<string>		additional_type_list = { ".css" };
	const	vector<string>		image_type_list = { ".jpg", ".tiff", ".png", ".jpeg", ".gif", ".ico", ".svg" };
	const	vector<string>		web_type_list = { ".html" };
	const	vector<string>		js_type_list = { ".js" };
	
	try
	{
		my_recv(inSSL, buf);

		cout << buf << "\n";

		Cleaning_refuse_in_buffer(buf, entire_query);

		// Parse page name
		Parse_name_page(entire_query.at(1), file_name, extension_file, flag);

		// If query GET
		if (entire_query.at(0) == "GET")
		{
			// Point position in string
			size_t value_search = entire_query.at(1).find('.');

			if (value_search == -1)
			{
				vector<string>::iterator key_find = find(entire_query.begin(), entire_query.end(), "Sec-WebSocket-Key:");

				if ((key_find - entire_query.begin()) != entire_query.size())
				{
					SR.websocket_handshake(inSSL, entire_query.at((key_find - entire_query.begin()) + 1));
					
					Cleanup_OpenSSL(inSSL);

					return 0;
				}

				//Response_default_html(inSSL);
				Send_response(inSSL, CD.get_file_from_db("index.html"));

				return 0;
			}

			file_name += extension_file;

			if (file_name == "chat.html" || file_name == "")
			{
				file_name = "index.html";

				Send_response(inSSL, CD.get_file_from_db(file_name));

				return 0;
			}

			size_t additional_type_list_size = additional_type_list.size();
			size_t image_type_list_size = image_type_list.size();
			size_t web_type_list_size = web_type_list.size();
			size_t js_type_list_size = js_type_list.size();

			for (size_t count = 0; count < additional_type_list_size; ++count)
				{
					if (extension_file == additional_type_list.at(count))
					{
						Send_response(inSSL, CD.get_file_from_db(file_name));

						return 0;
					}
				}

			for (size_t count = 0; count < image_type_list_size; ++count)
				{
					if (extension_file == image_type_list.at(count))
					{
						Send_response(inSSL, CD.get_file_from_db(file_name));

						return 0;
					}
				}

			// If the request is not an existing file .html that give home
			try
			{
				for (size_t count = 0; count < web_type_list_size; ++count)
				{
					if (extension_file == web_type_list.at(count))
					{
						Send_response(inSSL, CD.get_file_from_db(file_name));

						return 0;
					}
				}
			}
			catch (exception&)
			{
				Send_response(inSSL, CD.get_file_from_db(file_name));

				return 0;
			}

			for (size_t count = 0; count < js_type_list_size; ++count)
				{
					if (extension_file == js_type_list.at(count))
					{
						Send_response(inSSL, CD.get_file_from_db(file_name));

						return 0;
					}
				}
		}
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
	}

	return 0;
}


/*******************************************************************************/
void ResponseRequest::Cleaning_refuse_in_buffer(const string& InBuf, vector<string>& OutEntire_query)
{
	vector<string> temp_entire_query;

	size_t first_pos = 0;
	size_t last_pos = 0;

	while( (last_pos = InBuf.find("\r\n", first_pos)) != string::npos )
	{
		temp_entire_query.push_back( InBuf.substr(first_pos, (last_pos - first_pos)) );

		first_pos = last_pos+2;
	}

	temp_entire_query.push_back(InBuf.substr(first_pos));

	for(int i = 0; i < temp_entire_query.size(); ++i)
	{
		first_pos = 0;
		last_pos = 0;

		while( (last_pos = temp_entire_query.at(i).find(' ', first_pos)) != string::npos )
		{
			OutEntire_query.push_back( temp_entire_query.at(i).substr(first_pos, (last_pos - first_pos)) );

			first_pos = last_pos + 1;
		}	

		OutEntire_query.push_back(temp_entire_query.at(i).substr(first_pos));
	}	
}


/*******************************************************************************/
void ResponseRequest::Parse_name_page(string& inNamePage, string& outNamePage, string& outExtensionFile, bool& flag)
{
	size_t fileNameLen = inNamePage.size();

	string temp = inNamePage;

	bool Slash = false;

	flag = true;

	int countPoint = 1;

	int positionPoint = int();

	char token = char();

	for (int i = 0; i < fileNameLen; ++i)
	{
		token = temp[i];

		switch (token)
		{
			case '/':
			{
				Slash = true;
			}

			case '.':
			{
				positionPoint = i;
				++countPoint;
			}

			default:
			{
				
			}
		}
	}

	outNamePage = temp.erase(positionPoint, (fileNameLen - positionPoint));

	temp = inNamePage;

	outExtensionFile = temp.erase(0, positionPoint);

	if (Slash)
	{
		outNamePage.erase(0, 1);
	}
}