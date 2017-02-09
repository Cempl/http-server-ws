#pragma once


/*******************************************************************************/
// WEB Server
#include "Server.h"

// VDB SDK
#include <VSDK\VSDK2.h>


/*******************************************************************************/
FBL_Using_Namespace
VSQL_Using_Namespace
Std_Using_Namespace
VCLIENT_Using_Namespace


class ControlsDatabase
{
	public://////////////////////////////////////////////////////////////////////

		void InitValentina(int inCacheSize = 10 * 1024 * 1024);
		void ShutdownValentina(void);
		void OpenDB();
		void Flag_online();
		void AddAllFiles();

		bool FindAuthData(String inEmail, String inPass, String inToken);
		bool AddNewUser(String inName, String inEmail, String inPass);
		bool check_token(String inToken, string& outName);

		string Path_folder();
		string get_file_from_drive(string path);
		string get_file_from_db(string nameFile);

	protected:///////////////////////////////////////////////////////////////////

		I_Database_Ptr pVDB;
		I_SqlDatabase_Ptr pSqlVDB;

		bool gClient = true;
		bool gSSLEnable = false;
		bool gNotificationsEnable = false;

		I_Connection_Ptr gConn = NULL;
		I_Connection_Ptr gSqliteConn = NULL;
};

extern ControlsDatabase CD;