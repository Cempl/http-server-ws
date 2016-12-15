#pragma once


/*******************************************************************************/
#define port_connection 27015


/*******************************************************************************/
#include <conio.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <fstream>
#include <memory>
#include <vector>

// Socket
#include <winsock2.h>
#include <winsock.h>

// OpenSSL
#include <openssl\sha.h>
#include <openssl\bio.h>
#include <openssl\evp.h>
#include <openssl\buffer.h>
#include <openssl\rc4.h>

// Win .h
#include <Windows.h>
#include <stdexcept>
/*#include <vld.h>*/ // Visual Leak Detector

// My exception
#include "OtherExceptions.h"


/*******************************************************************************/
#pragma comment(lib, "WS2_32.lib")


/*******************************************************************************/
using namespace std;

/*******************************************************************************/
extern const uint16_t gLengthMessage;


/*******************************************************************************/
class Server
{
	public://////////////////////////////////////////////////////////////////////

		void start_server();


	protected:///////////////////////////////////////////////////////////////////

		WSADATA wsaData;// Информация о сокетах
		SOCKET server_socket;
		SOCKET client_socket;
		sockaddr_in storage_addresses;// Для хранения адреса


	protected:///////////////////////////////////////////////////////////////////

		void initialization_wsa();
		void create_socket();
		void bundle_socket_adresse();
		void listening_connection();
		void Processing_a_connection_request();
};