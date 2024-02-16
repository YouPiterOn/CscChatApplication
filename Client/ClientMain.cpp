#include "Client.h"

int main() {
	Client client;
	client.Start();
	client.ConnectToServer();
	client.CommunicationWithServer();
	return 0;
}