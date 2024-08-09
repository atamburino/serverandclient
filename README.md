# Server and Client Project

## Overview

This project is a simple implementation of a client-server model using C++ and Winsock API. The project demonstrates basic network communication between a server and multiple clients. The server can handle multiple clients simultaneously, and clients can send text messages, register usernames, and execute specific commands on the server.

## Features

- **Server Side:**
  - Accepts multiple client connections.
  - Supports commands from clients such as user registration, exit, and retrieving a list of connected users.
  - Outputs client messages to the server console.
  
- **Client Side:**
  - Connects to the server via IP address and port.
  - Allows users to input a username and send messages to the server.
  - Supports commands like registering the user, exiting the connection, and retrieving a list of connected users.

## Installation and Setup

### Prerequisites

- **Windows OS:** The project relies on the Winsock API, which is available on Windows.
- **C++ Compiler:** Ensure you have a C++ compiler that supports C++11 or higher.
- **Visual Studio:** The project was developed using Visual Studio, and you can use the provided `.sln` file to open the project.

### Building the Project

1. Clone the repository or download the project files.
2. Open the `serverandclient.sln` solution file in Visual Studio.
3. Build the solution by selecting `Build > Build Solution` or pressing `Ctrl + Shift + B`.
4. After a successful build, you will have two executables: one for the server and one for the client.

### Running the Server

1. Open a terminal and navigate to the directory containing the server executable.
2. Run the server by executing the command: `.\server.exe`.
3. Enter the port number and the maximum number of clients when prompted.

### Running the Client

1. Open a terminal and navigate to the directory containing the client executable.
2. Run the client by executing the command: `.\client.exe`.
3. Enter the server's IP address, port number, and your desired username when prompted.
4. Start sending messages or use the available commands.

## Commands

### Client Commands

- **$register:** Register your username with the server.
- **$exit:** Disconnect from the server.
- **$getlist:** Retrieve a list of connected users.
- **$getlog:** (Future implementation) Retrieve the server log.

### Server Commands

- The server primarily responds to commands sent by the clients and manages connections accordingly.

## File Structure

- **program.cpp:** Contains the main function that allows users to choose between running the server or client.
- **clientside.cpp:** Contains the implementation for the client's functionality, including connecting to the server and handling user input.
- **serverside.cpp:** Contains the implementation for the server's functionality, including accepting client connections and handling client requests.

## Future Improvements

- Implement logging functionality on the server side to handle the `$getlog` command.
- Add more robust error handling and input validation.
- Extend the client to support more complex commands and interactions.

## Contributing

If you would like to contribute to this project, please fork the repository and submit a pull request. Any contributions, such as bug fixes or new features, are welcome.
