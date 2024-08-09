// serverandclient.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "serverside.h"
#include "clientside.h"

int main()
{
    std::cout << "1. server \n2. client" << std::endl;

    int choice;
    std::cin >> choice;
    std::cin.ignore(1, '\n');

    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cout << "1. server \n2. client" << std::endl;
        std::cin >> choice;
    }

    if (choice == 1) {
        serverside server;
        server.start();
    }
    else if (choice == 2) {
        clientside client;
        client.start();
    }
}

