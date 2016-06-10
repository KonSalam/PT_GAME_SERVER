#include "Server.h"

void sent_message(std::vector<client_type> &client_array, int iResult, std::string msg)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (client_array[i].socket != INVALID_SOCKET)
			iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
	}
}

void disconnected(int id, Tank tanks[], int iResult, std::vector<client_type> &client_array)
{
	switch (id){
	case 0:
		tanks[0] = Tank(0, 0, 1);
		break;
	case 1:
		tanks[1] = Tank(0, 500, 2);
		break;
	case 2:
		tanks[2] = Tank(500, 0, 1);
		break;
	case 3:
		tanks[3] = Tank(500, 500, 2);
		break;
	}
	std::string msg = std::to_string(id) + " " + std::to_string(tanks[id].getX()) + " " + std::to_string(tanks[id].getY()) + " " + std::to_string(tanks[id].getCourse());
	sent_message(client_array, iResult, msg);
}

bool colision(Tank tanks[], int id)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == id) continue;

		if ((tanks[id].getX() >= tanks[i].getX() && tanks[id].getX()< (tanks[i].getX() + 50)) || (tanks[id].getX()>(tanks[i].getX() - 50) && tanks[id].getX() <= tanks[i].getX()))
		{
			if ((tanks[id].getY() >= tanks[i].getY() && tanks[id].getY() < (tanks[i].getY() + 50)) || (tanks[id].getY() > (tanks[i].getY() - 50) && tanks[id].getY() <= tanks[i].getY()))
			{
				return true;
			}
		}
	}
	return false;
}

void shoot(Tank tanks[], int id)
{
	int i;
	if (tanks[id].getCourse() == 1 || tanks[id].getCourse() == 2){
		i = tanks[id].getY();
	}
	else{
		i = tanks[id].getX();
	}

	if (tanks[id].getCourse() == 1 || tanks[id].getCourse() == 4){

		for (i; i <= 500; i += 5)
		{
			for (int j = 0; j < 4; j++)
			{
				//if (i == tanks[j].getY())
			}
		}
	}
	else{
		for (i; i >= 0; i -= 5)
		{
			for (int j = 0; j < 4; j++)
			{

			}
		}
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
					if (colision(tanks, new_client.id)){
						tanks[new_client.id].setY(tanks[new_client.id].getY() + 5);
					}
				}
				else if (strcmp(tempmsg, "80") == 0)//dol
				{
					tanks[new_client.id].setY(tanks[new_client.id].getY() + 5);
					tanks[new_client.id].setCourse(1);
					if (colision(tanks, new_client.id)){
						tanks[new_client.id].setY(tanks[new_client.id].getY() - 5);
					}
				}
				else if (strcmp(tempmsg, "75") == 0)//lewo
				{
					tanks[new_client.id].setX(tanks[new_client.id].getX() - 5);
					tanks[new_client.id].setCourse(3);
					if (colision(tanks, new_client.id)){
						tanks[new_client.id].setX(tanks[new_client.id].getX() + 5);
					}
				}
				else if (strcmp(tempmsg, "77") == 0)//prawo
				{
					tanks[new_client.id].setX(tanks[new_client.id].getX() + 5);
					tanks[new_client.id].setCourse(4);
					if (colision(tanks, new_client.id)){
						tanks[new_client.id].setX(tanks[new_client.id].getX() - 5);
					}
				}
				else if (strcmp(tempmsg, "115") == 0)//strzal
				{
					//shoot(tanks, new_client.id);
				}

				msg = std::to_string(new_client.id) + " " + std::to_string(tanks[new_client.id].getX()) + " " + std::to_string(tanks[new_client.id].getY()) + " " + std::to_string(tanks[new_client.id].getCourse());
				sent_message(client_array, iResult, msg);
			}
			else
			{
				msg = "Client #" + std::to_string(new_client.id) + " Disconnected";
				std::cout << msg << std::endl;
				disconnected(new_client.id, tanks, iResult, client_array);

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