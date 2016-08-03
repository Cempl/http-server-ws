#include "ControlsDatabase.h"


ControlsDatabase CD;


bool ControlsDatabase::open_connection_db()
{
	if (sqlite3_open("data_auten.dblite", &db))
	{
		cout << (stderr, "Error R/W/O DB: %s\n", sqlite3_errmsg(db));

		return false;
	}

	return true;
}

bool ControlsDatabase::close_connection_db()
{
	sqlite3_close(db);

	return true;
}

bool ControlsDatabase::creating_tables()
{
	open_connection_db();

	string t_sql = "CREATE TABLE IF NOT EXISTS users(login, pass)";

	char *SQL = (char*)t_sql.c_str();

	if (sqlite3_exec(db, SQL, callback, 0, &err))
	{
		cout << stderr << "Error SQL: " << err;

		sqlite3_free(err);
	}

	close_connection_db();

	return true;
}

bool ControlsDatabase::add_data_auth(string login, string pass)
{
	open_connection_db();

	string t_sql = "INSERT INTO USERS VALUES('" + login + "', '" + pass + "');";

	char *SQL = (char*)t_sql.c_str();

	if (sqlite3_exec(db, SQL, callback, 0, &err))
	{
		cout << stderr << "Error SQL: " << err;

		sqlite3_free(err);
	}

	close_connection_db();

	return true;
}

string ControlsDatabase::find_data_auth(string login)
{
	open_connection_db();

	string t_sql = "SELECT * from users;";

	char *SQL = (char*)t_sql.c_str();

	if (sqlite3_exec(db, SQL, callback, 0, &err))
	{
		cout << stderr << "Error SQL: " << err;

		sqlite3_free(err);
	}

	string tmp_key_user = "NULL";

	auto it_find = CD.map_db.begin();

	it_find = CD.map_db.find(login);
	
	if (it_find != CD.map_db.end())
	{
		tmp_key_user = it_find -> second; // returns pass
	}

	close_connection_db();

	return tmp_key_user;
}

string ControlsDatabase::delete_data_auth(string login)
{
	return string();
}

int ControlsDatabase::callback(void * data, int argc, char ** argv, char ** azColName)
{
	CD.map_db.insert(pair<string, string>(azColName[argc], argv[CD.column] ? argv[CD.column] : "NULL"));

	return 0;
}
