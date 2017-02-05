#ifndef _LogFile_H
	#define _LogFile_H
#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
#include <ctime>
#include <Shlwapi.h>


/*******************************************************************************/
class LogFile
{
	public://///////////////////////////////////////////////////////////////////

				LogFile()
				:
					local_time(),
					struct_local_time()	
				{};

				LogFile(const LogFile& InOther) = delete;

				~LogFile() {};

		void	write(const string& error);


	protected:///////////////////////////////////////////////////////////////////

		void path_initialization();
		void get_local_time();

	protected:///////////////////////////////////////////////////////////////////

		time_t	local_time = time_t();
		tm*		struct_local_time;	
		string path = string();
};

#endif // _LogFile_H