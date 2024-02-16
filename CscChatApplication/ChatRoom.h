#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <time.h>
#include <thread>
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <filesystem>
#include <list>
#include "Shared.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
using namespace std;

struct Client {
	Client(string name, SOCKET socket) : name(name), socket(socket) {}
	string name;
	SOCKET socket;
};

class ChatRoom
{
private:
	list<Client> _clients;
public:
	ChatRoom(string name) : name(name) {}

	string name = "";

	void AddClient(string clientName, SOCKET socket);
	void SendMessageToAll(string message, string senderName);
	void RemoveClient(string clientName);
	bool FindClient(string clientName);
};

