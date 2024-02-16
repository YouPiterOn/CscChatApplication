#pragma once
#include <string>
#include <vector>
#include <winsock2.h>
#include <mswsock.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

using namespace std;

namespace shared {
	void sendToSocket(string message, SOCKET reciever);

	string recieveFromSocket(SOCKET sender);
}
