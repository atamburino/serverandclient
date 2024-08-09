#include <WinSock2.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")
#include <unordered_map>

#pragma once
class serverside
{
	SOCKET listening;
	std::unordered_map<SOCKET, std::string> clients;
	fd_set master;
	int serverCap;

	bool connect();



public:
	void start();
};

