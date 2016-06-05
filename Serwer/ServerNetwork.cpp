#include "Server.h"

void disconnected(int id, Tank tanks[])
{
	switch (id){
	case 0:
		tanks[0] = Tank(0, 0, 1);
		break;
	case 1:
		tanks[0] = Tank(0, 500, 1);
		break;
	case 2:
		tanks[0] = Tank(500, 0, 2);
		break;
	case 3:
		tanks[0] = Tank(500, 500, 2);
		break;
	}
}

void sent_message(std::vector<client_type> &client_array, int iResult, std::string msg)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (client_array[i].socket != INVALID_SOCKET)
			iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
	}
}

int process_client(client_type &new_client, std::vector<client_type> &client_array, std::thread &thread, Tank tanks[])
{
	std::string msg = "";
	char tempmsg[DEFAULT_BUFLEN] = "";

	while (1)
	{
		memset(tempmsg, 0, DEFAULT_BUFLEN);

		if (new_client.socket != 0)
		{
			int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFLEN, 0);

			if (iResult != SOCKET_ERROR)
			{
				if (strcmp(tempmsg, "72") == 0)//gora
				{
					tanks[new_client.id].setY(tanks[new_client.id].getY() - 5);
					tanks[new_client.id].setCourse(2);
				}
				else if (strcmp(tempmsg, "80") == 0)//dol
				{
					tanks[new_client.id].setY(tanks[new_client.id].getY() + 5);
					tanks[new_client.id].setCourse(1);
				}
				else if (strcmp(tempmsg, "75") == 0)//lewo
				{
					tanks[new_client.id].setX(tanks[new_client.id].getX() - 5);
					tanks[new_client.id].setCourse(3);
				}
				else if (strcmp(tempmsg, "77") == 0)//prawo
				{
					tanks[new_client.id].setX(tanks[new_client.id].getX() + 5);
					tanks[new_client.id].setCourse(4);
				}
				else if (strcmp(tempmsg, "115") == 0)//strzal
				{

				}

				msg = std::to_string(new_client.id) + " " + std::to_string(tanks[new_client.id].getX()) + " " + std::to_string(tanks[new_client.id].getY());
				sent_message(client_array, iResult, msg);
			}
			else
			{
				msg = "Client #" + std::to_string(new_client.id) + " Disconnected";
				std::cout << msg << std::endl;
				disconnected(new_client.id, tanks);

				closesocket(new_client.socket);
				closesocket(client_array[new_client.id].socket);
				client_array[new_client.id].socket = INVALID_SOCKET;
				break;
			}
		}
	}
	thread.detach();
	return 0;
}