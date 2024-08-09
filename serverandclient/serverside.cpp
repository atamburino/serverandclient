#include "serverside.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

bool serverside::connect()
{
	SOCKET user = accept(listening, nullptr, nullptr);
	if (user == INVALID_SOCKET)
	{
		std::cout << "Failed\n";
		return false;
	}

	if (clients.size() < serverCap)
	{
		clients[user] = "empty";
		FD_SET(user, &master);
		return true;
	}
	else
	{
		shutdown(user, SD_BOTH);
		closesocket(user);
		return false;
	}
}

void serverside::start()
{
	WSADATA wsaData;

	// intialize winsock 
	int WSAResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (WSAResult != 0) {
		printf("WSAStartup failed: %d\n", WSAResult);
		return;
	}

	listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		return;
	}

	SOCKADDR_IN serverAddr;

	std::cout << "Please enter port" << std::endl;
	int port;
	std::cin >> port;
	while (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Please enter port" << std::endl;
		std::cin >> port;
	}

	std::cout << "enter server cap" << std::endl;
	std::cin >> serverCap;
	while (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "enter server cap" << std::endl;
		std::cin >> serverCap;
	}

	// set up server address 
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);
	if (serverAddr.sin_addr.s_addr == INADDR_NONE)
	{
		std::cout << "Invalid Client Address" << std::endl;
		return;
	}

	int result = bind(listening, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
	{
		std::cout << "err binding" << std::endl;
		return;
	}

	// Tell Winsock the socket is for listening 
	result = listen(listening, serverCap);
	if (result == SOCKET_ERROR)
	{
		std::cout << "err listening" << std::endl;
		return;
	}

	std::cout << "server online" <<std::endl;


	FD_ZERO(&master);
	FD_SET(listening, &master);

	bool running = true;
	while (running)
	{
		fd_set copy = master;
		timeval now;
		now.tv_sec = 1;


		int activesockets = select(0, &copy, nullptr, nullptr, &now);
		for (size_t i = 0; i < activesockets; i++)
		{
			SOCKET active = copy.fd_array[i];
			if (active == listening)
			{
				if (connect())
				{
					std::cout << "Connected" << std::endl;
				}
				else
				{
					std::cout << "Failed to Connect" << std::endl;
				}
			}
			else
			{
				char messagetype;
				int result = recv(active, &messagetype, 1, 0);
				if (result < 1)
				{
					//future change - socket 2 username & eliminate user 
					std::cout << clients[active] << " disconnected\n";
					return;
				}



				if (messagetype == 't')
				{
					char length;
					result = recv(active, &length, 1, 0);
					if (result < 1)
					{
						//future change - socket 2 username & eliminate user 
						std::cout << clients[active] << " disconnected\n";
						return;
					}
					int len = static_cast<int>(length);
					char* hold = new char[len];
					result = recv(active, hold, len, 0);
					if (result < 1)
					{
						//future change - socket 2 username & eliminate user 
						std::cout << clients[active] << " disconnected\n";
						return;
					}
					std::string incoming = hold;
					delete[] hold;
					std::cout << clients[active] << ">" << incoming << std::endl;
				}
				else if (messagetype == 'c')
				{

					char command;
					result = recv(active, &command, 1, 0);
					if (result < 1)
					{
						//future change - socket 2 username & eliminate user 
						std::cout << clients[active] << " disconnected\n";
						return;
					}

					if (command == 'r')
					{
						char length;
						result = recv(active, &length, 1, 0);
						if (result < 1)
						{
							//future change - socket 2 username & eliminate user 
							std::cout << clients[active] << " disconnected\n";
							return;
						}
						char* hold = new char[length];
						result = recv(active, hold, length, 0);
						if (result < 1)
						{
							//future change - socket 2 username & eliminate user 
							std::cout << clients[active] << " disconnected\n";
							return;
						}
						clients[active] = hold;
						delete[] hold;

					}
					else if (command == 'e')
					{
						for (std::unordered_map<SOCKET, std::string> ::iterator iter = clients.begin(); iter != clients.end(); iter++)
						{
							if (iter->first == active)
							{
								//future change - socket 2 username & eliminate user
								std::cout << clients[active] << " disconnected\n";

								shutdown(active, SD_BOTH);
								closesocket(active);
								FD_CLR(active, &master);

								clients.erase(iter);
								break;
							}
						}
					}
					else if (command == 'l')
					{
						std::string listino;
						std::string sentlistino;
						for (std::unordered_map<SOCKET, std::string> ::iterator iter = clients.begin(); iter != clients.end(); iter++)
						{
							listino += clients[active];
							listino += ' ';
						}
						listino += '\0';
						sentlistino += static_cast<char>(listino.size()); 

						sentlistino += listino;
						sentlistino += '\0';
						int result = send(active, sentlistino.c_str(), sentlistino.size(), 0);

						if (result < 1)
						{ 
							std::cout << clients[active] << " no list\n";
							return;
						}
					}
					else if (command == 'o')
					{
						// get log 
						//output text to consol from file

					}
				}
			}
		}
	}

	// Close the socket
	closesocket(listening);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}
