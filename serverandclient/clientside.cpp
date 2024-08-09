#include "clientside.h"
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>


void clientside::start()
{
	WSADATA wsaData;

	// intialize winsock 
	int WSAResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (WSAResult != 0) {
		printf("WSAStartup failed: %d\n", WSAResult);
		return;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		return;
	}


	SOCKADDR_IN clientAddr;

	std::cout << "Please enter IP" << std::endl;
	std::string ipAddress;
	std::cin >> ipAddress;
	std::cin.ignore(1, '\n');
	while (std::cin.fail()){
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Please enter IP" << std::endl;
		std::cin >> ipAddress;
	}

	std::cout << "Please enter port" << std::endl;
	int port;
	std::cin >> port;
	std::cin.ignore(1, '\n');
	while (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Please enter port" << std::endl;
		std::cin >> port;
	}

	std::cout << "Please enter user" << std::endl;
	std::string username;
	std::cin >> username;
	std::cin.ignore(1, '\n');
	while (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Please enter user" << std::endl;
		std::cin >> username;
	}

	// set up server address 
	clientAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ipAddress.c_str(), &clientAddr.sin_addr);
	clientAddr.sin_port = htons(port);
	if (clientAddr.sin_addr.s_addr == INADDR_NONE)
	{
		std::cout << "Invalid Client Server Address" << std::endl;
		return;
	}

	// Connect to server
	int connResult = connect(clientSocket, (sockaddr*)&clientAddr, sizeof(clientAddr));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		return;
	}
	bool running = true;
	while (running)
	{
		std::cout << username << ">";
		std::string output;
		std::getline(std::cin, output);
		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cout << username << ">";
			std::cin >> output;
		}
		if (output.c_str()[0] == '$') {
			//command logic
			std::string sent;

			sent += 'c'; //command
			if (std::strstr(output.c_str(), "$register") != nullptr)
			{
				sent += 'r'; // register
				sent += static_cast<char>(username.size() + 1);
				sent += username;
				sent += '\0';

				int result = send(clientSocket, sent.c_str(), sent.size(), 0);
				if (result <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}
			}
			else if (std::strstr(output.c_str(), "$exit") != nullptr)
			{
				sent += 'e'; //exit
				sent += '\0';

				int result = send(clientSocket, sent.c_str(), sent.size(), 0);
				if (result <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}
				return;
			}
			else if (std::strstr(output.c_str(), "$getlist") != nullptr)
			{
				sent += 'l'; //getlist
				sent += '\0';

				int result = send(clientSocket, sent.c_str(), sent.size(), 0);
				if (result <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}

				int messageLength;
				char msglen;

				int recvResult = recv(clientSocket, &msglen, 1, 0);
				if (recvResult <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}
				messageLength = static_cast<int>(msglen);


				char* temp = new char[messageLength];
				recvResult = recv(clientSocket, temp, messageLength, 0);
				if (recvResult <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();;
				}
				std::cout << temp << '\n';
				delete[](temp);
			}
			else if (std::strstr(output.c_str(), "getlog") != nullptr)
			{
				sent += 'o'; //getlog
				sent += '\0';

				int result = send(clientSocket, sent.c_str(), sent.size(), 0);
				if (result <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}

				int messageLength;
				int recvResult = recv(clientSocket, (char*)&messageLength, 1, 0);
				if (recvResult <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}


				char* temp = new char[messageLength];
				recvResult = recv(clientSocket, temp, messageLength, 0);
				if (recvResult <= 0)
				{
					std::cerr << "Command err" << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}
				std::cout << temp << '\n';
				delete[](temp);
			}
			else
			{
				std::cout << "Not valid. \n";
			}

		}
		else {
			std::string sent;
			sent += 't';
			int sentLength = output.size()+1;
			sent += static_cast<char>(sentLength);
			sent += output.c_str();
			sent += '\0';


			int result = send(clientSocket, sent.c_str(), sent.size(), 0);

				if (result <= 0)
				{
					std::cerr << "Encountered an error when sending text. WSA Error #:" << WSAGetLastError() << '\n';
					shutdown(clientSocket, SD_BOTH);
					closesocket(clientSocket);
					WSACleanup();
				}
		}
	}
}
