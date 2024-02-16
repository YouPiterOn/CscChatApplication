#include "Server.h"

void Server::Start() {
	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
	{
		cerr << "WSAStartup failed" << endl;
	}
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == INVALID_SOCKET)
	{
		cerr << "Error creating socket: " << WSAGetLastError() << endl;
		WSACleanup();
	}
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	if (bind(_serverSocket, reinterpret_cast<sockaddr*>(&_serverAddr), sizeof(_serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Bind failed with error: " << WSAGetLastError() << endl;
		closesocket(_serverSocket);
		WSACleanup();
	}
}

void Server::ListenForClient() {
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cerr << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(_serverSocket);
		WSACleanup();
	}
	list<thread> threads;
	threads.push_back(thread(&Server::MessageSender, this));
	while (true) {
		SOCKET clientSocket = accept(_serverSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET)
		{
			cerr << "Accept failed with error: " << WSAGetLastError() << endl;
		}
		threads.push_back(thread(&Server::CommunicationWithClient, this, ref(clientSocket)));
	}
}

void Server::CommunicationWithClient(SOCKET clientSocket) {
	string clientName = shared::recieveFromSocket(clientSocket);
	string roomName = shared::recieveFromSocket(clientSocket);
	

	mtx.lock();
	ChatRoom* clientRoom = FindRoom(roomName);
	if (clientRoom == nullptr) {
		CreateRoom(roomName);
		clientRoom = FindRoom(roomName);
	}

	while (clientRoom->FindClient(clientName) || clientName == "") {
		shared::sendToSocket("Room already has client with this name, enter different: ", clientSocket);
		clientName = shared::recieveFromSocket(clientSocket);
	}
	clientRoom->AddClient(clientName, clientSocket);
	shared::sendToSocket("You joined room " + clientRoom->name, clientSocket);
	mtx.unlock();

	string message;
	while (true) {
		message = shared::recieveFromSocket(clientSocket);
		if (message.size() <= 0) continue;
		
		if (message == "LEAVE") {
			clientRoom->RemoveClient(clientName);

			shared::sendToSocket("Enter room name to join: ", clientSocket);
			roomName = shared::recieveFromSocket(clientSocket);

			mtx.lock();
			clientRoom = FindRoom(roomName);
			if (clientRoom == nullptr) {
				CreateRoom(roomName);
				clientRoom = FindRoom(roomName);
			}
			while (clientRoom->FindClient(clientName)) {
				shared::sendToSocket("Room already has client with this name, enter different: ", clientSocket);
				clientName = shared::recieveFromSocket(clientSocket);
			}
			clientRoom->AddClient(clientName, clientSocket);
			shared::sendToSocket("You joined room " + clientRoom->name, clientSocket);
			mtx.unlock();
		}
		else {
			mtx.lock();
			_messageQueue.push(Message(clientName, message, clientRoom));
			mtx.unlock();
		}
	}
}

void Server::MessageSender() {
	while (true) {
		if (!_messageQueue.empty()) {
			mtx.lock();
			Message m = _messageQueue.front();
			_messageQueue.pop();
			m.room->SendMessageToAll(m.messageText, m.clientName);
			mtx.unlock();
		}
	}
}

ChatRoom* Server::FindRoom(string roomName) {
	ChatRoom* clientRoom;
	for (auto room = _rooms.begin(); room != _rooms.end(); room++) {
		if (room->name == roomName) {
			clientRoom = &*room;
			return clientRoom;
		}
	}
	return nullptr;
}

void Server::CreateRoom(string roomName) {
	_rooms.push_back(ChatRoom(roomName));
}
