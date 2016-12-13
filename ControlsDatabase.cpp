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
		catch (xException& e)
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
	I_Database_Ptr pVDB = pSqlVDB->get_BaseDatabase();

	const char* inDbName = "userAuthData";
	const char* pDbNameOnly = ExtractNameFromFullPath(inDbName);

	I_Location_Ptr pLocation = CreateRawLocation(pDbNameOnly);

	pVDB->put_Location(pLocation);
	pVDB->Open();
}


/*******************************************************************************/
bool ControlsDatabase::FindAuthData(String inLogin, String inPass)
{
	I_Cursor_Ptr pVdbCursor;

	I_Field_Ptr nameField;

	String getEmail = String();
	String getName = String();
	String getPass = String();

	bool res = false;

	pVdbCursor = pSqlVDB->SqlSelect("SELECT *FROM UserAuthData");

	if (pVdbCursor->FirstRecord())
	{
		do
		{
			nameField = pVdbCursor->get_Field("user_email");
			getEmail = nameField->get_Value()->get_String();

			nameField = pVdbCursor->get_Field("user_name");
			getName = nameField->get_Value()->get_String();

			if (inLogin == getEmail || inLogin == getName)
			{
				nameField = pVdbCursor->get_Field("user_password");
				getPass = nameField->get_Value()->get_String();

				res = (inPass == getPass);

				// End of cycle
				break;
			}
		} 
		while (pVdbCursor->NextRecord());
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
	catch (...)
	{
		return false;
	}

	return true;
}