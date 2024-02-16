#include "ChatRoom.h"

void ChatRoom::SendMessageToAll(string message, string senderName) {
	string composedMessage = senderName + ": " + message;
	for (Client c : _clients) {
		if (c.name != senderName) {
			shared::sendToSocket(composedMessage, c.socket);
		}
	}
}

void ChatRoom::AddClient(string name, SOCKET socket) {
	Client newClient(name, socket);
	_clients.push_back(newClient);

	ChatRoom::SendMessageToAll("Joined room", name);
}

void ChatRoom::RemoveClient(string name) {
	for (auto itr = _clients.begin(); itr != _clients.end(); itr++) {
		if (itr->name == name) {
			_clients.erase(itr);
			ChatRoom::SendMessageToAll("Left room", name);
		}
	}
}