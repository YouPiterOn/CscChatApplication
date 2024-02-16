#include "ChatRoom.h"

void ChatRoom::SendMessageToAll(string message, string senderName) {
	string composedMessage = senderName + ": " + message;
	for (Client c : _clients) {
		if (c.name != senderName) {
			shared::sendToSocket(composedMessage, c.socket);
		}
	}
}

void ChatRoom::AddClient(string clientName, SOCKET socket) {
	_clients.push_back(Client(clientName, socket));
	ChatRoom::SendMessageToAll("Joined room "+name, clientName);
}

void ChatRoom::RemoveClient(string clientName) {
	for (auto itr = _clients.begin(); itr != _clients.end(); itr++) {
		if (itr->name == clientName) {
			_clients.erase(itr);
			ChatRoom::SendMessageToAll("Left room "+name, clientName);
			return;
		}
	}
}

bool ChatRoom::FindClient(string clientName) {
	for (auto itr = _clients.begin(); itr != _clients.end(); itr++) {
		if (itr->name == clientName) {
			return TRUE;
		}
	}
	return FALSE;
}