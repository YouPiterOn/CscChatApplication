#include "Server.h"

int main() {
	Server server;
	server.Start();
	server.ListenForClient();
	return 0;
}