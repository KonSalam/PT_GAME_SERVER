#include "Server.h"
#include <thread>

Server::Server()
{
	tanks[0] = Tank(0, 0, 1);
	tanks[1] = Tank(0, 500, 2);
	tanks[2] = Tank(500, 0, 1);
	tanks[3] = Tank(500, 500, 2);
	server_socket = INVALID_SOCKET;
	num_clients = 0;
	temp_id = -1;
}

void Server::init()
{
	client.resize(MAX_CLIENTS);
	std::cout << "Intializing Winsock..." << std::endl;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Setup Server
	std::cout << "Setting up server..." << std::endl;
	getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &server);

	//Create a listening socket for connecting to server
	std::cout << "Creating server socket..." << std::endl;
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

	//Setup socket options
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

	//Assign an address to the server socket.
	std::cout << "Binding socket..." << std::endl;
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);

	//Listen for incoming connections.
	std::cout << "Listening..." << std::endl;
	listen(server_socket, SOMAXCONN);

	//Initialize the client list
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		client[i] = { -1, INVALID_SOCKET };
	}
}

void Server::run()
{
	std::string msg = "";
	while (1)
	{
		SOCKET incoming = INVALID_SOCKET;
		incoming = accept(server_socket, NULL, NULL);
		if (incoming == INVALID_SOCKET) continue;

		//Reset the number of clients
		num_clients = -1;
		//Create a temporary id for the next client
		temp_id = -1;

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (client[i].socket == INVALID_SOCKET && temp_id == -1)
			{
				if (tanks[i].getLife() != 0){
					client[i].socket = incoming;
					client[i].id = i;
					temp_id = i;
				}
			}
			if (client[i].socket != INVALID_SOCKET)
				num_clients++;
		}

		if (temp_id != -1)
		{
			//Send the id to that client
			std::cout << "Client #" << client[temp_id].id << " Accepted" << std::endl;
			msg = std::to_string(client[temp_id].id);
			send(client[temp_id].socket, msg.c_str(), strlen(msg.c_str()), 0);

			//Create a thread process for that client
			my_thread[temp_id] = std::thread(process_client, std::ref(client[temp_id]), std::ref(client), std::ref(my_thread[temp_id]), tanks);
			Sleep(180);
			for (int i = 0; i < 4; i++){
				if (tanks[i].getLife() != 0){
					msg = std::to_string(i) + " " + std::to_string(tanks[i].getX()) + " " + std::to_string(tanks[i].getY()) + " " + std::to_string(tanks[i].getCourse()) + " " + std::to_string(0);
				}
				else{
					msg = std::to_string(i) + " " + std::to_string(tanks[i].getX()) + " " + std::to_string(tanks[i].getY()) + " " + std::to_string(tanks[i].getCourse()) + " " + std::to_string(3);
				}
				//std::cout <<msg<< std::endl;
				Sleep(400);
				send(client[temp_id].socket, msg.c_str(), strlen(msg.c_str()), 0);
			}
		}
		else
		{
			msg = "Server is full";
			send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << msg << std::endl;
		}
	}

	closesocket(server_socket);
	//Close client socket
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		my_thread[i].detach();
		closesocket(client[i].socket);
	}

	//Clean up Winsock
	WSACleanup();
	std::cout << "Program has ended successfully" << std::endl;
}