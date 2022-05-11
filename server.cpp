#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")
#define SERVER_PORT 3820

int index = -1;

using namespace std;

SOCKET socket_arr[256];
// HANDLE handle_arr[256];

DWORD WINAPI soketsoket(LPVOID lpParam) {

    int index_soc = *(int*)lpParam;

    char buf_in[20];
    char exit[20] = "s";
    char buf_out[34] = "Response from server";
    int bytes = 0;
    while (1) {
        bytes = recv(socket_arr[index_soc], (char *)buf_in, sizeof(buf_in), 0);

        if (bytes == SOCKET_ERROR) {
            cout << "bytes == SOCKET_ERROR" << endl;
            break;
        }

        cout << buf_in << endl;

        for (int i = 0; i <= index; ++i) {
            send(socket_arr[i], (char *)buf_out, sizeof(buf_out), 0);
        }

        if (buf_in[0] == 's')
            break;
    }
    return 0;
}

int main() {
    setlocale(LC_CTYPE, "Russian");

    struct sockaddr_in SrvAddr;
    SOCKET SrvSock, Conn;

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult < 0) {
        cout << "WSAStartup failed" << endl;
        getchar();
        exit(0);
    }

    SrvSock = socket(AF_INET, SOCK_STREAM, 0);

    SrvAddr.sin_family = AF_INET;
    SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    SrvAddr.sin_port = htons(SERVER_PORT);

    bind(SrvSock, (sockaddr *)&SrvAddr, sizeof SrvAddr);

    listen(SrvSock, 5);

    struct sockaddr_in ConnAddr;
    memset(&ConnAddr, 0, sizeof(ConnAddr));

    cout << "...Server is running..." << endl;
    int AddrLen = sizeof(ConnAddr);

    DWORD ThreadId;
    INT ThreadParameter = 19;
    HANDLE hThread;

    while (1) {
        Conn = accept(SrvSock, (struct sockaddr *) &ConnAddr, &AddrLen);
        if (Conn == INVALID_SOCKET) {
            cout << "Conn == INVALID_SOCKET" << endl;
        } else {
            index++;
            socket_arr[index] = Conn;
        }
        
        hThread = CreateThread(
            NULL,              // атрибуты безопасности по умолчанию
            0,                 // размер стека по умолчанию  
            soketsoket,        // указатель на процедуру создаваемого потока
            &index,            // аргумент, передаваемый функции потока
            0,                 // флаги создания по умолчанию
            &ThreadId);        // возвращаемый идентификатор потока
        if (hThread == NULL) cout << endl << "CreateThread failed" << endl;
    }

    CloseHandle(hThread);

    shutdown(Conn, 2);
    closesocket(Conn);

    std::cout << endl << "Server down" << endl;
    getchar();
    return 0;
}