#include <iostream>
#include "Server.h"
int main()
{
	Server server;
	server.init();
	server.run();
	system("pause");
	return 0;
}