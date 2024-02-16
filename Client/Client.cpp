#include "Client.h"

void Client::Start() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "WSAStartup failed" << endl;
	}
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Error creating socket: " << WSAGetLastError() << endl;
		WSACleanup();
	}
}

void Client::ConnectToServer() {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	InetPton(AF_INET, serverIp, &serverAddr.sin_addr);
	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Connect failed with error: " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
	}

	string name = "";
	cout << "Enter client name: ";
	cin >> name;
	shared::sendToSocket(name, clientSocket);
	string room = "";
	cout << "Enter room name to join: ";
	cin >> room;
	shared::sendToSocket(room, clientSocket);
}

void Client::CommunicationWithServer() {
	thread sendThread(&Client::Sender, this);
	thread recieveThread(&Client::Reciever, this);
	sendThread.join();
	recieveThread.join();
}

void Client::Sender() {
	string message;
	while (true) {
		getline(cin, message);
		shared::sendToSocket(message, clientSocket);
	}
}

void Client::Reciever() {
	string message;
	while (true) {
		message = shared::recieveFromSocket(clientSocket);
		mtx.lock();
		cout << message << endl;
		mtx.unlock();
	}
}
