#include <iostream>
#include <string>
#include<ws2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
void main() {

	string ipAddress = "127.0.0.1";// IP Adress of the server 
	int port = 54000;       // Listening port # on the server
	// initialize winsock

	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "can't start winsock, Err #" << wsResult << endl;
		WSACleanup();
		return;
	
	}
	// create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // use AF_INET6 for ipv 6
	if (sock == INVALID_SOCKET) {
		cerr << "can't create socket, Err #" << WSAGetLastError() << endl;
		return;
	}

	// fill in a hint structure

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	//connect to server
	int connResult = connect(sock, (sockaddr*) &hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}
	// D0-while loop to sennd and recieve data
	char buf[4096];
	string userInput;
	do {
	// prompt the user for some text
		cout << "input text;" << endl;
		getline(cin, userInput);
		if (userInput.size() > 0) // make sure the user has typed in something
		{
			// send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0) {
				// echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				
				}
			}

		}
	} while (userInput.size() > 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
