#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")// подключили библиотеку
#define SERVER_PORT 3820

using namespace std;

DWORD WINAPI soketsoket(LPVOID lpParam) {

    SOCKET Conn=*(SOCKET*) lpParam;

    char buf_in[20];
    char exit[20] = "s";
    char buf_out[34] = "Hello from Server!";
    int bytes = 0;
    while (1)
    {

        bytes = recv(Conn, (char *)buf_in, sizeof(buf_in), 0);//принял информацию в буфер
        std::cout << buf_in << endl;

        if (bytes == SOCKET_ERROR) {
            std::cout << "not sent";
            break;

        }
        if (buf_in[0] == 's') {
            send(Conn, (char *)exit, sizeof(exit), 0);
            break;
        }
        send(Conn, (char *)buf_out, sizeof(buf_out), 0);
    }
    return 0;
}

int main()
{
    setlocale(LC_CTYPE, "Russian"); // включаем русскую кодировку

    struct sockaddr_in SrvAddr;     // Адресная структура сервера



    SOCKET SrvSock, Conn;

    WSADATA wsaData;  // для работы Windows с сокетами
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // запустили библиотеку
    if (iResult < 0)
    {
        std:: cout << "error" << endl;
        getchar();
        exit(0);
    }


    SrvSock = socket(AF_INET, SOCK_STREAM, 0);//Создаем  сокет сервера

    //Задаем адрес сервера
    SrvAddr.sin_family = AF_INET;
    SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// 127.0.0.1
    SrvAddr.sin_port = htons(SERVER_PORT);
    //Настраиваем сокет
    bind(SrvSock, (sockaddr *)&SrvAddr, sizeof SrvAddr);

    //Слушаем
    listen(SrvSock, 5);

    struct sockaddr_in ConnAddr;  // Адресная структура клиента
    memset(&ConnAddr, 0, sizeof(ConnAddr));

    printf("...Server is running...\n");
    int AddrLen = sizeof(ConnAddr);
    char buf_in[20];
    char exit[20] = "s";
    char buf_out[34] = "Hello from server!";
    int nsize;
    int bytes = 0;
    bool stop = false;

    DWORD ThreadId;
    INT ThreadParameter = 19;
    HANDLE hThread;

    while (1) {
        Conn = accept(SrvSock, (struct sockaddr *) &ConnAddr, &AddrLen);
        //Ждем очередного клиента
        

        
        // создаем поток
        hThread = CreateThread(
            NULL,              // атрибуты безопасности по умолчанию
            0,                 // размер стека по умолчанию  
            soketsoket,        // указатель на процедуру создаваемого потока
            &Conn,  // аргумент, передаваемый функции потока
            0,                 // флаги создания по умолчанию
            &ThreadId);        // возвращаемый идентификатор потока

        if (hThread == NULL)  printf("CreateThread failed.");

        
        
    }
    CloseHandle(hThread);


        shutdown(Conn, 2); // запретили передачу и прием сообщений
        closesocket(Conn); //закрыли=разрушили сокет

    
    std:: cout << endl << "Out of server program" << endl;
    getchar();
    return 0;
}