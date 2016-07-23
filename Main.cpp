/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
void main()
{
	Server first_server;

	try
	{
		first_server.start_server();
	}
	catch(...)
	{
		cout << "Yeah!";
	}

	system("pause");
}