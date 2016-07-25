#include "LogFile.h"


void LogFile::write(const string& error)
{
	get_local_time();
	path_initialization();

	stringstream buf;
	char* day = new char [2];
	buf << struct_local_time->tm_mday;
	buf >> day;

	ofstream fout(path + "\\" + day + ".log", ios_base::app);

		delete[] day;

		fout << struct_local_time->tm_mday << "-" << struct_local_time->tm_mon + 1 << "-" << struct_local_time->tm_year + 1900 << "\n"
			 << struct_local_time->tm_hour << ":" << struct_local_time->tm_min << ":" << struct_local_time->tm_sec << "\n"
			 << error << "\n\n";

	fout.close();
}


void LogFile::get_local_time()
{
	local_time = time(NULL);
	struct_local_time = localtime(&local_time);
}


void LogFile::path_initialization()
{
	char* temp_path = new char [256];
	stringstream buf;

	GetModuleFileName(NULL, (LPTSTR)temp_path, 256);
	PathRemoveFileSpec((LPTSTR)temp_path);

	path = temp_path;
	buf << struct_local_time->tm_mon + 1 << " " << struct_local_time->tm_year + 1900;

	buf >> temp_path;
	path += "\\";
	path += temp_path;
	path += "-";
	buf >> temp_path;
	path += temp_path;

	CreateDirectory((LPCTSTR)path.c_str(), NULL);

	delete[] temp_path;
}