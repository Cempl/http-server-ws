#pragma once

/*******************************************************************************/
#include <condition_variable>
#include <thread>
#include <queue>
#include <mutex>

/*******************************************************************************/
#include "Server.h"

/*******************************************************************************/
using namespace std;

/*******************************************************************************/
//struct my_date_base
//{
//		thread::id my_id;
//		char my_buffer[512];
//};
//
///*******************************************************************************/
//vector<my_date_base> gList;
//mutex list_mutex;
//condition_variable data_cond;
//
///*******************************************************************************/
//void send_data(SOCKET& client_socket);
//void recv_data(my_date_base& date);