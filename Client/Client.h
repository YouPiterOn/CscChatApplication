#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include "Shared.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

using namespace std;

class Client
{
	WSADATA wsaData;

	const int port = 12345;
	const PCWSTR serverIp = L"127.0.0.1";
	SOCKET clientSocket;
	sockaddr_in serverAddr;
	mutex mtx;

public:
	void Start();
	void ConnectToServer();
	void CommunicationWithServer();
	void Reciever();
	void Sender();
};

