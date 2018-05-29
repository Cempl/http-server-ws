#ifndef _Server_H
	#define _Server_H
#pragma once


/*******************************************************************************/
#define PORT_CONNECTION				443
#define PUBL_CERT		"127.0.0.1.crt"
#define PRIV_KEY		"127.0.0.1.key" 


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

// WSLexer
#include "WSLexer.h"

// Socket
#include <winsock2.h>
#include <winsock.h>
#include <fcntl.h>

// OpenSSL
#include <openssl\sha.h>
#include <openssl\ssl.h>
#include <openssl\bio.h>
#include <openssl\buffer.h>
#include <openssl\evp.h>
#include <openssl\err.h>
#include <openssl\rc4.h>

// Win .h
#include <Windows.h>
#include <stdexcept>
//#include <vld.h> // Visual Leak Detector

// My exception
#include "OtherExceptions.h"


/*******************************************************************************/
#pragma comment(lib, "WS2_32.lib")


/*******************************************************************************/
using namespace std;

/*******************************************************************************/
extern const uint16_t gLengthMessage;


int ParseHttpHEAD( SSL* inSSL );


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
		SSL_CTX *myContextOpenSSL; // OpenSSL context
		SSL *mySSL;
		struct sockaddr_in addr;
		int addrLen = sizeof(addr);


	protected:///////////////////////////////////////////////////////////////////

		void initialization_wsa();
		void create_socket();
		void bundle_socket_adresse();
		void listening_connection();
		void Processing_a_connection_request();


	protected:///////////////////////////////////////////////////////////////////

		void configure_openssl_context(SSL_CTX *inCtx);
		void init_openssl();
		SSL_CTX *create_openssl_context();
};

#endif // _Server_H