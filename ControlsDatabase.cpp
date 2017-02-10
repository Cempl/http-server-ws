/*******************************************************************************/
#include "ControlsDatabase.h"


/*******************************************************************************/
ControlsDatabase CD;


/*******************************************************************************/
void ControlsDatabase::InitValentina(int inCacheSize)
{
	if (gClient) // We asked to run VSDK example in the client/server mode.
	{
		ValentinaInitC(); // we init Valentina Client. It DO NOT needs any serials.

		if (gSSLEnable) // check if we want SSL connection
		{
			// VServer listens for Valentina SSL connections on port 15434 on default.
			gConn = CreateConnection("localhost", "sa", "sa", "UTF-8", 15434);
			gConn->UseSSL();
		}
		else // no SSL connection.
		{
			gConn = CreateConnection("localhost", "sa", "sa", "UTF-8");
		}

		if (gNotificationsEnable)
			gConn->UseNotifications();

		try
		{
			gConn->Open();
		}
		catch (xException&)
		{
			std::cout	<< "You running Examples in gClient = true mode." << std::endl
						<< "Make sure that you have install Valentina Server." << std::endl;
		}
	}
	else // LOCAL mode, so we init VKERNEL engine.
	{
		ValentinaInit(inCacheSize); // HERE you can put your VSDK serials.
	}
}


/*******************************************************************************/
void ControlsDatabase::ShutdownValentina(void)
{
	if (gClient)
	{
		gConn->Close();
		ValentinaShutDownC();
	}
	else
	{
		ValentinaShutDown();
	}
}


/*******************************************************************************/
void ControlsDatabase::OpenDB()
{
	pSqlVDB = CreateSqlDatabase(gConn);
	pVDB = pSqlVDB->get_BaseDatabase();

	const char* inDbName = "userAuthData";
	const char* pDbNameOnly = ExtractNameFromFullPath(inDbName);

	I_Location_Ptr pLocation = CreateRawLocation(pDbNameOnly);

	pVDB->put_Location(pLocation);
	pVDB->Open();
}


/*******************************************************************************/
bool ControlsDatabase::FindAuthData(String inLogin, String inPass, String inToken)
{
	bool res = false;

	String Query = "SELECT *FROM UserAuthData WHERE user_email = '" + inLogin + "';";

	try
	{
		I_Cursor_Ptr pVdbCursor = pSqlVDB->SqlSelect(Query);

		Query = "SELECT *FROM UserAuthData WHERE user_password = '" + inPass + "';";

		pVdbCursor = pSqlVDB->SqlSelect(Query);

		pVdbCursor = pSqlVDB->SqlSelect("UPDATE UserAuthData SET token = '" + inToken + "' WHERE user_email = '" + inLogin + "';");

		res = true;
	}
	catch (xException&)
	{
		// not found
	}

	return res;
}


/*******************************************************************************/
void ControlsDatabase::Flag_online()
{

}


/*******************************************************************************/
bool ControlsDatabase::AddNewUser(String inName, String inEmail, String inPass)
{
	try
	{
		pSqlVDB->SqlExecute(
			"INSERT INTO UserAuthData(user_name, user_email, user_password) VALUES ('" 
			+ inName + "', '" 
			+ inEmail + "', '" 
			+ inPass + "');");
	}
	catch (xException&)
	{
		return false;
	}

	return true;
}


/*******************************************************************************/
void ControlsDatabase::AddAllFiles()
{
	vector<string> files;
	vector<string> names;

	// html
	files.push_back("\\html\\authorization.html");
	names.push_back("authorization.html");
	files.push_back("\\html\\chat.html");
	names.push_back("chat.html");
	files.push_back("\\html\\FullHome.html");
	names.push_back("FullHome.html");
	files.push_back("\\html\\home.html");
	names.push_back("home.html");
	files.push_back("\\html\\index.html");
	names.push_back("index.html");

	// js
	files.push_back("\\js\\chat.js");
	names.push_back("chat.js");
	files.push_back("\\js\\index.js");
	names.push_back("index.js");
	files.push_back("\\js\\jquery_min.js");
	names.push_back("jquery_min.js");
	files.push_back("\\js\\loadContent.js");
	names.push_back("loadContent.js");
	files.push_back("\\js\\sha512min.js");
	names.push_back("sha512min.js");

	// css
	files.push_back("\\css\\authorization.css");
	names.push_back("authorization.css");
	files.push_back("\\css\\chat.css");
	names.push_back("chat.css");
	files.push_back("\\css\\index.css");
	names.push_back("index.css");

	// picture
	files.push_back("\\image\\favicon.ico");
	names.push_back("favicon.ico");

	string path_to_file = Path_folder();

	I_Table_Ptr pTable = pVDB->get_Table("AllFiles");
	
	I_Field_Ptr pField_1 = pTable->get_Field("html/css/js");
	I_Field_Ptr pField_2 = pTable->get_Field("fileName");

	I_FldBlob_Ptr pBlob = fbl_dynamic_cast<I_FldBlob>(pField_1);
	
	int i = 0;
	pTable->SetBlank(forUpdate);

	if (pTable->FirstRecord())
	{
		do
		{
			pTable->SetBlank(forUpdate);

			string str(get_file_from_drive(path_to_file + files[i]));

			pBlob->WriteData(str.c_str(), (vuint32)str.size());
			pField_2->get_Value(forUpdate)->put_String(names[i].c_str());

			pTable->UpdateRecord();

			++i;

		} while (pTable->NextRecord());
	}
}


/*******************************************************************************/
string ControlsDatabase::get_file_from_db(string nameFile)
{
	String getName;
	String tNameFile(nameFile.c_str());
	string res = string();

	nameFile = nameFile.c_str();

	I_Cursor_Ptr pVdbCursor = pSqlVDB->SqlSelect("SELECT *FROM AllFiles");
	I_Field_Ptr pField;
	
	if (pVdbCursor->FirstRecord())
	{
		do
		{
			pField = pVdbCursor->get_Field("fileName");
			getName = pField->get_Value()->get_String();

			if (tNameFile == getName)
			{
				pField = pVdbCursor->get_Field("html/css/js");

				I_FldBlob_Ptr pFBlob = fbl_dynamic_cast<I_FldBlob>(pField);

				const int size = pFBlob->get_DataSize();
				MemPtr<char> buff(size);
				const int read = pFBlob->ReadData(buff, size);

				res = string(buff, (size_t)read);
				// End of cycle
			}
		} while (pVdbCursor->NextRecord());
	}
	
	return res;
}


/*******************************************************************************/
string ControlsDatabase::Path_folder()
{
	char szPath[MAX_PATH] = {};

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	// Pointer to the last occurrence of "\"
	char *lstChr = strrchr(szPath, '\\');

	// replaced by zero (truncate)
	*lstChr = '\0';

	return szPath;
}


/*******************************************************************************/
string ControlsDatabase::get_file_from_drive(string path)
{
	ifstream fin(path, ios_base::binary);

	if (!fin.is_open())
	{
		throw exception(("Missing " + path).c_str());
	}
	
	string res((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

	fin.close();

	return res;
}


/*******************************************************************************/
bool ControlsDatabase::check_token(String inToken, string& outName)
{
	bool res = false;

	String currToken;
	String Name;
	String Query = "SELECT *FROM UserAuthData WHERE token = '" + inToken + "';";

	try
	{
		I_Cursor_Ptr pVdbCursor = pSqlVDB->SqlSelect(Query);
		Name = pVdbCursor->get_Field("user_name")->get_Value()->get_String();
		outName = string(Name.getBufferA(), Name.length());

		res = true;
	}
	catch (xException&)
	{
		// not found
	}

	return res;
}