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
#define IP_ADDRESS "127.0.0.1"
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
	SOCKET server_socket = INVALID_SOCKET;
	std::vector<client_type> client;
	std::thread my_thread[MAX_CLIENTS];

	int num_clients = 0;
	int temp_id = -1;
	Tank tanks[MAX_CLIENTS] = { Tank(0, 0), Tank(0, 500), Tank(500, 0), Tank(500, 500) };;

public:
	void init();//inicjazliacja servera
	void run();
};


#endif