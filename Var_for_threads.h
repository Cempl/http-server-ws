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
extern vector<string> my_gList;
extern mutex glist_mutex;
extern condition_variable gdata_cond;


