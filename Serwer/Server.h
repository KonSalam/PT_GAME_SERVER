#ifndef Server_H
#define Server_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include "Tank.h"

#pragma comment (lib, "Ws2_32.lib")
#define IP_ADDRESS "192.168.1.107"
#define DEFAULT_PORT "2556"
#define DEFAULT_BUFLEN 1024

struct client_type
{
	int id;
	SOCKET socket;
};

const char OPTION_VALUE = 1;
const int MAX_CLIENTS = 4;

class Server
{
private:
	WSADATA wsaData;
	struct addrinfo hints;
	struct addrinfo *server = NULL;
	SOCKET server_socket;
	std::vector<client_type> client;
	std::thread my_thread[MAX_CLIENTS];
	Tank tanks[MAX_CLIENTS];

	int num_clients;
	int temp_id;
	
public:
	Server();
	void init();//inicjazliacja servera
	void run();
};


#endif