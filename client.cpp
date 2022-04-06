#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

int main() {

	// Messages
	const std::string ERR_MSG = "Error: ";
	const std::string INF_MSG = "Info: ";
	const std::string WR_MSG = "Warning: ";

	//Key constants
	const char SERVER_ADDR[] = "localhost";					// Enter IPv4 address of Server
	const short SERVER_PORT = 3820;				// Enter Listening port on Server side
	const short BUFF_SIZE = 1024;					// Maximum size of buffer for exchange info between server and client

	int iResult;

	WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult < 0) {
		std::cout << ERR_MSG << "WinSock initialization was failed" << '\n';
		std::cout << ERR_MSG << WSAGetLastError() << '\n';
		getchar();
		exit(0);
	} else
		std::cout << INF_MSG << "WinSock initialization is OK" << '\n';

	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET) {
		std::cout << ERR_MSG << "WinSock initialization was failed" << '\n';
		std::cout << ERR_MSG << WSAGetLastError() << '\n'; 
		closesocket(ClientSock);
		WSACleanup();
		getchar();
		exit(0);
	}
	else
		std::cout << INF_MSG << "Client socket initialization is OK" << '\n';

	struct sockaddr_in Addr;
	ZeroMemory(&Addr, sizeof(Addr));

	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	Addr.sin_port = htons(SERVER_PORT);

	iResult = connect(ClientSock, (struct sockaddr*)&Addr, sizeof(Addr));

	if (iResult == INVALID_SOCKET) {
		std::cout << ERR_MSG << "Unable connect to remote server" << '\n';    
		getchar();	
		exit(0);
	} else 
		std::cout << INF_MSG << "Connection to remote server was successfully done" << '\n';

	
	std::vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Buffers for sending and receiving data
	short packet_size = 0;

	while (true) {

		std::cout << "Your (Client) message to Server: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		// Check whether client like to stop chatting 
		if (clientBuff[0] == 'e' && clientBuff[1] == 'o' && clientBuff[2] == 'l') {
			shutdown(ClientSock, SD_BOTH);
			closesocket(ClientSock);
			WSACleanup();
			std::cout << INF_MSG << "Connection disabled" << '\n';
			getchar();	
			exit(0);
		}

		packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << ERR_MSG << "Can't send message to Server. Error # " << WSAGetLastError() << '\n';
			closesocket(ClientSock);
			WSACleanup();
			getchar();	
			exit(0);
		} 

		packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << ERR_MSG << "Can't receive message from Server. Error # " << WSAGetLastError() << '\n';
			closesocket(ClientSock);
			WSACleanup();
			getchar();	
			exit(0);
		}
		else
			std::cout << "Server message: " << servBuff.data() << '\n';

	}

	closesocket(ClientSock);
	WSACleanup();
	std::cout << '\n' << "Exit from the client part" << '\n';
	getchar();

	return 0;
}
