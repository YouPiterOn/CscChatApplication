#include "Shared.h"

void shared::sendToSocket(string message, SOCKET reciever) {
	const char messageSize = message.size();
	send(reciever, &messageSize, 1, 0);
	send(reciever, message.c_str(), message.size(), 0);
}

string shared::recieveFromSocket(SOCKET sender) {
	char messageSize = 0;
	recv(sender, &messageSize, 1, 0);
	if ((int)messageSize <= 0) return "";
	vector<char> message((int)messageSize);
	recv(sender, &message[0], (int)messageSize, 0);
	string str = "";
	str.append(message.cbegin(), message.cend());
	return str;
}