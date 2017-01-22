/*******************************************************************************/
#include "ResponseRequest.h"
#include "LogFile.h"
#include "ControlsDatabase.h"
#include "Encryption.h"
#include "WSLexer.h"


/*******************************************************************************/
int ResponseRequest::Request(SOCKET client_socket)
{
			bool				flag = bool();

			string				buf(12288, 0);	
			string				file_name = { 0 };
			string				extension_file = { 0 };
			string				not_supported_method = "method_is_not_supported.html";

			vector<string>		entire_query;
	const	vector<string>		additional_type_list = { ".css" };
	const	vector<string>		image_type_list = { ".jpg", ".tiff", ".png", ".jpeg", ".gif", ".ico" };
	const	vector<string>		web_type_list = { ".html" };
	const	vector<string>		js_type_list = { ".js" };
	
	try
	{
		my_recv(client_socket, buf);

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
					SR.websocket_handshake(client_socket, entire_query.at((key_find - entire_query.begin()) + 1));

					closesocket(client_socket);

					return 0;
				}

				Response_default_html(client_socket);
				closesocket(client_socket);

				return 0;
			}

			file_name += extension_file;

			if (file_name == "chat.html" || file_name == "")
			{
				file_name = "index.html";

				Response_html(client_socket, file_name);
				closesocket(client_socket);

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
						Response_css(client_socket, file_name);
						closesocket(client_socket);

						return 0;
					}
				}

			for (size_t count = 0; count < image_type_list_size; ++count)
				{
					if (extension_file == image_type_list.at(count))
					{
						Response_image(client_socket, file_name);
						closesocket(client_socket);

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
						Response_html(client_socket, file_name);
						closesocket(client_socket);

						return 0;
					}
				}
			}
			catch (exception& e)
			{
				Response_default_html(client_socket);
				closesocket(client_socket);

				return 0;
			}

			for (size_t count = 0; count < js_type_list_size; ++count)
				{
					if (extension_file == js_type_list.at(count))
					{
						Response_js(client_socket, file_name);
						closesocket(client_socket);

						return 0;
					}
				}
		}

		// If query POST
		if (entire_query.at(0) == "POST")
		{
			// It may JS not have time to perform calculations and sends an POST empty query (size empty query 518)
			if (buf.size() == 518)
			{
				file_name = "index.html";

				Response_html(client_socket, file_name);
				closesocket(client_socket);

				return 0;
			}

			if (file_name != "index.html" || file_name == "")
			{
				string inLogin = string();
				string inName = string();
				string inPass = string();

				static int secret_key = int();

				// Search in body data from DH
				Search_DH_gen_and_mod(entire_query, secret_key, client_socket);

				// Search auth data in the body of request
				Search_auth_data(entire_query, inLogin, inPass, secret_key, client_socket, file_name);

				// Search reg data in the body of request
				Search_reg_data(entire_query, inName, inLogin, inPass, secret_key, client_socket, file_name);
			}
			else
			{
				file_name = "index.html";

				Response_html(client_socket, file_name);
				closesocket(client_socket);

				return 0;
			}
		}

		string method_http = entire_query.at(0);

		//Skip empty queries and if query less minimum query (HTTP)
		if (entire_query.size() <= 5)
		{
			return 0;
		}

		// Show unsupported methods
		if (method_http == "OPTIONS" ||
			method_http == "HEAD" ||
			method_http == "PUT" ||
			method_http == "PATCH" ||
			method_http == "DELETE" ||
			method_http == "TRACE" ||
			method_http == "CONNECT")
		{
			Response_html(client_socket, not_supported_method);
			closesocket(client_socket);
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

			first_pos = last_pos+1;
		}	

		OutEntire_query.push_back(temp_entire_query.at(i).substr(first_pos));
	}	
}


/*******************************************************************************/
void ResponseRequest::Search_auth_data(vector<string>& entire_query, string& inLogin, string& inPass, int& secret_key, SOCKET& client_socket, string& file_name)
{
	size_t entire_query_size = entire_query.size();
	size_t index_auth_login = size_t();

	vector<int> inData(3072);
 	vector<int> decrypt(3072);
	vector<int> key(3072);
	vector<int> leftBlock(1536);
	vector<int> rightBlock(1536);
	vector<int>	arrayRounderKeys(24576);

	// Amount rounds encryption
	int amoundRounds = 16;

	// Hashing secret key (DH)
	key = Enc.hashingKey(secret_key);

	// Generate array riunder keys
	arrayRounderKeys = Enc.arrayKeys(key, amoundRounds);

	// Search in the login request
	for (size_t i = 0; i < entire_query_size; ++i)
	{
		size_t pos = entire_query[i].find("AuthLogin=");

		if (pos != string::npos)
		{
			index_auth_login = i + 1;

			inLogin = entire_query[i].erase(0, 10);

			inData = Enc.strBinaryToIntBinary(inLogin);

			decrypt = Enc.decryptFeistelNetwork(inData, amoundRounds, arrayRounderKeys);

			inLogin = Enc.vectorCharToString(decrypt);
		}
	}

	// Vectors cleaning
	decrypt.clear();
	inData.clear();
	leftBlock.clear();
	rightBlock.clear();

	// Search in the pass request
	for (size_t i = index_auth_login; i < entire_query_size; ++i)
	{
		size_t pos = entire_query[i].find("AuthPass=");

		if (pos != string::npos)
		{
			index_auth_login = i + 1;

			inPass = entire_query[i].erase(0, 9);
			
			inData = Enc.strBinaryToIntBinary(inPass);

			decrypt = Enc.decryptFeistelNetwork(inData, amoundRounds, arrayRounderKeys);

			inPass = Enc.vectorCharToString(decrypt);
		}
	}

	if (inLogin.size() != 0 && inPass.size() != 0)
	{
		// Copy STD string in FBL string
		String inLogin(inLogin.c_str());
		String inPass(inPass.c_str());

		// Check the authorization data in DB
		bool authData = CD.FindAuthData(inLogin, inPass);

		if (authData)
		{
			file_name = "chat.html";
			Response_html(client_socket, file_name);
			closesocket(client_socket);
		}
		else
		{
			file_name = "index.html";
			Response_html(client_socket, file_name);
			closesocket(client_socket);
		}
	}
}


/*******************************************************************************/
void ResponseRequest::Search_reg_data(vector<string>& entire_query,string& inName, string& inLogin, string& inPass, int& secret_key, SOCKET& client_socket, string& file_name)
{
	size_t entire_query_size = entire_query.size();
	size_t index = size_t();

	vector<int> inData(3072);
	vector<int> decrypt(3072);
	vector<int> key(3072);
	vector<int> leftBlock(1536);
	vector<int> rightBlock(1536);
	vector<int>	arrayRounderKeys(24576);

	// Amount rounds encryption
	int amoundRounds = 16;

	// Hashing secret key (DH)
	key = Enc.hashingKey(secret_key);

	// Generate array riunder keys
	arrayRounderKeys = Enc.arrayKeys(key, amoundRounds);

	// Search in the name request
	for (size_t i = 0; i < entire_query_size; ++i)
	{
		size_t pos = entire_query[i].find("RegName=");

		if (pos != string::npos)
		{
			index = i + 1;

			inName = entire_query[i].erase(0, 8);

			inData = Enc.strBinaryToIntBinary(inName);

			decrypt = Enc.decryptFeistelNetwork(inData, amoundRounds, arrayRounderKeys);

			inName = Enc.vectorCharToString(decrypt);
		}
	}

	// Vectors cleaning
	decrypt.clear();
	inData.clear();
	leftBlock.clear();
	rightBlock.clear();

	// Search in the login request
	for (size_t i = index; i < entire_query_size; ++i)
	{
		size_t pos = entire_query[i].find("RegLogin=");

		if (pos != string::npos)
		{
			index = i + 1;

			inLogin = entire_query[i].erase(0, 9);

			inData = Enc.strBinaryToIntBinary(inLogin);

			decrypt = Enc.decryptFeistelNetwork(inData, amoundRounds, arrayRounderKeys);

			inLogin = Enc.vectorCharToString(decrypt);
		}
	}

	// Vectors cleaning
	decrypt.clear();
	inData.clear();
	leftBlock.clear();
	rightBlock.clear();

	// Search in the password request
	for (size_t i = index; i < entire_query_size; ++i)
	{
		size_t pos = entire_query[i].find("RegPass=");

		if (pos != string::npos)
		{
			index = i + 1;

			inPass = entire_query[i].erase(0, 8);

			inData = Enc.strBinaryToIntBinary(inPass);

			decrypt = Enc.decryptFeistelNetwork(inData, amoundRounds, arrayRounderKeys);

			inPass = Enc.vectorCharToString(decrypt);
		}
	}

	if (inName.size() != 0 && inLogin.size() != 0 && inPass.size() != 0)
	{
		// Copy STD string in FBL string
		String inName(inName.c_str());
		String inLogin(inLogin.c_str());
		String inPass(inPass.c_str());

		// Add registration data in DB
		bool regData = CD.AddNewUser(inName, inLogin, inPass);

		if (regData)
		{
			file_name = "chat.html";
			Response_html(client_socket, file_name);
			closesocket(client_socket);
		}
		else
		{
			file_name = "index.html";
			Response_html(client_socket, file_name);
			closesocket(client_socket);
		}
	}
}


/*******************************************************************************/
void ResponseRequest::Search_DH_gen_and_mod(vector<string>& entire_query, int& secret_key, SOCKET& client_socket)
{
	size_t entire_query_size = entire_query.size();

	string gen = string();
	string mod = string();
	string gen_mod = string();
	string in_res_mod = string();

	for (size_t i = 0; i < entire_query_size; ++i)
	{
		size_t pos = entire_query.at(i).find("DH_proto");

		if (pos != string::npos)
		{
			gen_mod = entire_query[i].erase(0, 8);

			size_t gen_size = gen_mod.size();
			size_t p_ampersant = entire_query.at(i).find('&');

			gen = gen_mod;
			mod = gen_mod;
			in_res_mod = gen_mod;

			gen = gen.erase(p_ampersant, (gen_size - p_ampersant));
			mod = mod.erase(0, p_ampersant);
			in_res_mod = mod.erase(0, 1);
			p_ampersant = in_res_mod.find('&');

			in_res_mod = in_res_mod.erase(0, p_ampersant + 1);

			p_ampersant = mod.find('&');
			size_t tmp_size_mod = mod.size();
			mod = mod.erase(p_ampersant, (tmp_size_mod - p_ampersant));

			int int_gen = stoi(gen);
			int int_mod = stoi(mod);

			int int_in_res_mod = stoi(in_res_mod);

			int private_number = int();

			int out_mod_res = Enc.get_mod_and_gen(int_gen, int_mod, private_number);
			
			// send res_mod
			Send_DH_res_mod(client_socket, out_mod_res);

			secret_key = Enc.calculated_mod(int_in_res_mod, int_mod, private_number);
		}
	}
}


/*******************************************************************************/
void ResponseRequest::Send_DH_res_mod(SOCKET& client_socket, int& out_mod_res)
{
	int result = int();

	string response = string();
	string double_str;

	ostringstream t_buff;

	t_buff << fixed << setprecision(0) << (out_mod_res);

	double_str = t_buff.str();

	response += "HTTP/1.1 200 OK\n";
	response += "Server: VaV/V2\n";
	response += "Content-Type: text/html;\n";
	response += "X-Powered-By: c++\r\n\r\n";
	response += double_str;

	Send_response(client_socket, response, result);

	closesocket(client_socket);
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