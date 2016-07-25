/*******************************************************************************/
#include "Server.h"
#include "LogFile.h"


/*******************************************************************************/
void main()
{
	Server first_server;

	try
	{
		first_server.start_server();
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
	}

	system("pause");
}