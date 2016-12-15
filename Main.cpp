/*******************************************************************************/
#include "Server.h"
#include "LogFile.h"
#include "ControlsDatabase.h"


/*******************************************************************************/
void main()
{
	Server first_server;

	try
	{
		CD.InitValentina();

		CD.OpenDB();

		first_server.start_server();
	}
	catch(OtherExceptions& e)
	{
		LogFile log;
		log.write(e.what());
	}

	system("pause");

	CD.ShutdownValentina();
}