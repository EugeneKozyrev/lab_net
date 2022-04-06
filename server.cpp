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

	// Key constants
	const char SERVER_ADDR[] = "localhost";			// Enter local Server IP address
	const int SERVER_PORT = 3820;				// Enter Open working server port
	const short BUFF_SIZE = 1024;			// Maximum size of buffer for exchange info between server and client

	// Custom addres cannot be used
	// DWORD ip = inet_addr(SERVER_ADDR); 

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


	SOCKET SrvSock = socket(AF_INET, SOCK_STREAM, 0);

	if (SrvSock == INVALID_SOCKET) {
		std::cout << ERR_MSG << "WinSock initialization was failed" << '\n';
		std::cout << ERR_MSG << WSAGetLastError() << '\n'; 
		closesocket(SrvSock);
		WSACleanup();
		getchar();
		exit(0);
	}
	else
		std::cout << INF_MSG << "Server socket initialization is OK" << '\n';


	struct sockaddr_in SrvAddr;
	ZeroMemory(&SrvAddr, sizeof(SrvAddr));

	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	SrvAddr.sin_port = htons(SERVER_PORT);

	iResult = bind(SrvSock, (sockaddr *)&SrvAddr, sizeof SrvAddr);

	if ( iResult != 0 ) {
		std::cout << ERR_MSG << "Socket binding to server info. Error # " << WSAGetLastError() << '\n';
		closesocket(SrvSock);
		WSACleanup();
		getchar();
		exit(0);
	}
	else 
		std::cout << INF_MSG << "Binding socket to Server info is OK" << '\n';

	iResult = listen(SrvSock, 5); 

	if ( iResult != 0 ) {
		std::cout << ERR_MSG << "Can't start to listen to. Error # " << WSAGetLastError() << '\n';
		closesocket(SrvSock);
		WSACleanup();
		getchar();
		exit(0);
	}
	else 
		std::cout << " ------- ...Server is up to state... ------- " << '\n';

	struct sockaddr_in ConnAddr;
	ZeroMemory(&ConnAddr, sizeof(ConnAddr));
	
	int ConnAddr_size = sizeof(ConnAddr);

	SOCKET Conn = accept(SrvSock, (struct sockaddr *) &ConnAddr, &ConnAddr_size);;

	if (Conn == INVALID_SOCKET) {
		std::cout << ERR_MSG << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << '\n';
		closesocket(SrvSock);
		closesocket(Conn);
		WSACleanup();
		getchar();
		exit(0);
	} else
		std::cout << INF_MSG << "Connection to a client established successfully" << '\n';

	std::vector<char> servBuff(BUFF_SIZE);
	std::vector<char> clientBuff(BUFF_SIZE);

	short packet_size = 0;

	while(true) {
		packet_size = recv(Conn, servBuff.data(), servBuff.size(), 0);
		std::cout << "Client's message: " << servBuff.data() << '\n';

		std::cout << "Your (host) message: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		if (clientBuff[0] == 'e' && clientBuff[1] == 'o' && clientBuff[2] == 'l') {
			shutdown(Conn, SD_BOTH);
			closesocket(SrvSock);
			closesocket(Conn);
			WSACleanup();
			std::cout << INF_MSG << "Connection disabled" << '\n';
			getchar();
			exit(0);
		}

		packet_size = send(Conn, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << ERR_MSG << "Can't send message to Client. Error # " << WSAGetLastError() << '\n';
			closesocket(SrvSock);
			closesocket(Conn);
			WSACleanup();
			getchar();
			exit(0);
		}
	}

	closesocket(SrvSock);
	closesocket(Conn);
	WSACleanup();

	std::cout << '\n' << INF_MSG <<  "Exit from the server part" << '\n';
	getchar();
	
	return 0;
}
