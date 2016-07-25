#pragma once

#include "Server.h"

#include <ctime>
#include <Shlwapi.h>




class LogFile
{
	public:

				LogFile() {};
				LogFile(const LogFile& InOther) = delete;
				~LogFile() {};
		void	write(const string& error);


	protected:

		void path_initialization();
		void get_local_time();

	protected:

		time_t	local_time;
		tm*		struct_local_time;	
		string path;
};