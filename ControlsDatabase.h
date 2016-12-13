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

		void InitValentina(int inCacheSize = 8 * 1024 * 1024);
		void ShutdownValentina(void);
		void OpenDB();
		void Flag_online();

		bool FindAuthData(String inEmail, String inPass);
		bool AddNewUser(String inName, String inEmail, String inPass);

	protected:///////////////////////////////////////////////////////////////////

		I_SqlDatabase_Ptr pSqlVDB;

		bool gClient = true;
		bool gSSLEnable = false;
		bool gNotificationsEnable = false;

		I_Connection_Ptr gConn = NULL;
		I_Connection_Ptr gSqliteConn = NULL;
};

extern ControlsDatabase CD;