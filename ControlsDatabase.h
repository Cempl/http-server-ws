#pragma once


#include "Server.h"


class ControlsDatabase
{
	public://////////////////////////////////////////////////////////////////////

		bool open_connection_db();
		bool close_connection_db();
		bool add_data_auth(string login, string pass);
		bool creating_tables();
		string find_data_auth(string login);
		string delete_data_auth(string login);


	protected:///////////////////////////////////////////////////////////////////

		static int callback(void *data, int argc, char **argv, char **azColName);

	protected:///////////////////////////////////////////////////////////////////

		map<string, string> map_db;
		sqlite3		*db = 0; // хэндл объекта соединение к БД
		char		*err = 0;
		const int	column = 1;
};

		extern ControlsDatabase CD;