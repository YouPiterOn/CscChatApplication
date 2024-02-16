#pragma once
#include <string>
#include <fstream>
#include <thread>
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <filesystem>
#include <list>
#include <mutex>
#include <queue>
#include "ChatRoom.h"
#include "Shared.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

using namespace std;
namespace fs = filesystem;

struct Message {
	Message(string clientName, string messageText, ChatRoom* room) : clientName(clientName), messageText(messageText), room(room) {}
	string clientName;
	string messageText;
	ChatRoom* room;
};

class Server
{
	WSADATA _wsaData;

	const int _port = 12345;
	SOCKET _serverSocket;
	sockaddr_in _serverAddr;

	mutex mtx;

	list<ChatRoom> _rooms;
	queue<Message> _messageQueue;

public:
	void Start();
	void ListenForClient();
	void CommunicationWithClient(SOCKET clientSocket);
	void MessageSender();
	ChatRoom* FindRoom(string roomName);
	void CreateRoom(string roomName);
};

