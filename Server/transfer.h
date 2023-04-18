#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include"values.h"
#include"head.h"
#include"ioFile.h"
#include<iostream>
#include<string>
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

class Transfer {
public:
	SOCKET clientSocket;
	ioFile file;

	Transfer() {};
	Transfer(SOCKET clientSocket) { this->clientSocket = clientSocket; };

	int recvData(AppProtocol *ap) {
		int dataSize = 0;
		char size[4];
		initAP(ap);

		dataSize = recv(clientSocket, ap->type, 10, 0);
		if (dataSize < 0)
			cout << "recv type error: " << WSAGetLastError() << endl;

		dataSize = recv(clientSocket, &ap->end, 1, 0);
		if (dataSize < 0)
			cout << "recv end error: " << WSAGetLastError() << endl;

		dataSize = recv(clientSocket, size, 4, 0);
		if (dataSize < 0)
			cout << "recv size error: " << WSAGetLastError() << endl;
		ap->fileSize = cptoi(size);

		dataSize = recv(clientSocket, ap->filePath, 260, 0);
		if (dataSize < 0)
			cout << "recv path error: " << WSAGetLastError() << endl;

		dataSize = recv(clientSocket, (char*)ap->data, BuffSize, 0);
		if (dataSize < 0)
			cout << "recv size error: " << WSAGetLastError() << endl;

		return 0;
	}

	int sendData(AppProtocol *ap) {
		int dataSize = 0;
		char size[4];

		dataSize = send(clientSocket, ap->type, 10, 0);
		if(dataSize<0)
			cout << "send type error:" << WSAGetLastError() << endl;

		dataSize = send(clientSocket, &ap->end, 1, 0);
		if(dataSize<0)
			cout << "send end error:" << WSAGetLastError() << endl;

		itocp(ap->fileSize, size);
		dataSize = send(clientSocket, size, 4, 0);
		if(dataSize<0)
			cout << "send size error:" << WSAGetLastError() << endl;

		dataSize = send(clientSocket, ap->filePath, 260, 0);
		if(dataSize<0)
			cout << "send path error:" << WSAGetLastError() << endl;

		dataSize = send(clientSocket, (char*)ap->data, BuffSize, 0);
		if(dataSize<0)
			cout << "send data error:" << WSAGetLastError() << endl;

		return 0;
	}

	int parProtocol(AppProtocol *ap) {
		string type = ap->type;
		if (type == "cmd") { return 1; }
		else if (type == "upFile") { return 2; }
		else if (type == "dowFile") { return 3; }
		else if (type == "discon") { return 4; }
		else if (type == "exit") { return -1; }
		else { cout << "parsing protocol error!\n"; return 5; }
	}
};
