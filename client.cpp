#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")
#define SERVER_PORT 3820

using namespace std;

DWORD WINAPI Soket_listener(LPVOID lpParam) {
    SOCKET ClientSock = *(SOCKET*)lpParam;
    int bytes = 0;
    char rez[34];
    while (1) {
        // Получили сообщение
        bytes = recv(ClientSock, (char*)rez, sizeof(rez), 0);
        cout << "\n*** Soket_listener ***" << endl;
        if (bytes == SOCKET_ERROR) {
            cout << endl << "bytes == SOCKET_ERROR" << endl;
            break;
        }
        else cout << rez << endl;
    }
    return 0;
}

int main() {
    setlocale(LC_CTYPE, "Russian");

    SOCKET ClientSock = INVALID_SOCKET;
    WSADATA WSStartData;
    struct sockaddr_in Addr;

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult < 0) {
        cout << endl << "WSAStartup failed" << endl;
        getchar();
        exit(0);
    }


    ClientSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&Addr, 0, sizeof(Addr));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    Addr.sin_port = htons(SERVER_PORT);

    HANDLE hThread;
    DWORD ThreadId;

    char buf[20];

    int u = connect(ClientSock, (struct sockaddr*)&Addr, sizeof(Addr));
    if (u == INVALID_SOCKET) {
        cout << "u == INVALID_SOCKET" << endl;    
        getchar();    
        return 0;
    }
    else {
        cout << "Connected" << endl;
        hThread = CreateThread(NULL, 0, Soket_listener, &ClientSock, 0, &ThreadId);
        if (hThread == NULL) cout << endl << "CreateThread failed" << endl;
        while (1) {
            cout << "input: ";
            fgets(buf, sizeof(buf), stdin);
            if (buf[0] == 's') break;
            send(ClientSock, (char*)buf, sizeof(buf), 0);
        }
    }

    shutdown(ClientSock, 2);
    closesocket(ClientSock);
    WSACleanup();
    cout << "Client down" << endl;
    getchar();
    return 0;
}