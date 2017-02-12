#ifndef _ControlsDatabase_H
	#define _ControlsDatabase_H
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

							ControlsDatabase() {};
							ControlsDatabase(const ControlsDatabase& inOther) = delete;
							~ControlsDatabase() {};

		void				AddAllFiles();
		void				Flag_online();
		void				InitValentina(int inCacheSize = 10 * 1024 * 1024);
		void				OpenDB();
		void				ShutdownValentina(void);

		bool				AddNewUser(String inName, String inEmail, String inPass);
		bool				check_token(String inToken, string& outName);
		bool				FindAuthData(String inEmail, String inPass, String inToken);

		string				get_file_from_db(string nameFile);
		string				get_file_from_drive(string path);
		string				Path_folder();

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

#endif // _ControlsDatabase_H