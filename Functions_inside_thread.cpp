#include "Server.h"
#include "Var_for_threads.h"
#include "Interrupt.h"

/*******************************************************************************/
vector<string> my_gList;
mutex glist_mutex;
condition_variable gdata_cond;

/*******************************************************************************/
void send_data(SOCKET& client_socket)
{		
	int index = 0;

	while(true)
	{
		unique_lock<mutex> u_mutex(glist_mutex);

			/*gdata_cond.wait(u_mutex, [&index]{return (index < my_gList.size());});*/
			interruptible_wait(gdata_cond, u_mutex, [&index]{return (index < my_gList.size());});
			my_send(client_socket, my_gList[index]);
			++index;

		u_mutex.unlock();
	}

}

/*******************************************************************************/
int recv_data(string& data)
{
	int result = websocket_get_content(data, data.size()); // Раскодировали

	if(result == 1)
	{
		return 1;
	}

	websocket_set_content(data, data.size(), result); // Перекодировали сообщение

	lock_guard<mutex> guard(glist_mutex); 

	my_gList.push_back(data); // Записали сообщение в очередь
	gdata_cond.notify_all(); // Будим потоки для отправки сообщения
	
	return 0;
}