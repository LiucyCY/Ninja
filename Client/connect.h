#pragma once
#pragma warning(disable:4996)
#include"values.h"
#include<stdio.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Connect {
public:
	int sockErr = 0;
	unsigned short wVersionRequested = socketVersion;
	WSADATA stWSAData;
	SOCKET listenSocket,clientSocket;
	sockaddr_in addIP;

	int initConnect() { 
		sockErr = WSAStartup( wVersionRequested, &stWSAData);
		if (sockErr != 0) {
			unsigned short version = stWSAData.wVersion;
			unsigned short highest = stWSAData.wHighVersion;
			printf("version: %hu\nhighestVersion: %hu\n", version, highest);
			printf("sockErr: %d\n", sockErr);
		}

		addIP.sin_family = af;
		addIP.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
		addIP.sin_port = htons(PORT);

		listenSocket = socket(af, type, protocol);
		if (listenSocket == INVALID_SOCKET) {
			printf("LastError:%d\n", WSAGetLastError());
			cleanConnect();
			return -1;
		}
		return 0;
	};

	SOCKET createConnect() { 
		int iResult = connect( listenSocket, (sockaddr*)&addIP, sizeof(sockaddr_in));
		if (iResult != 0) {
			listenSocket = INVALID_SOCKET;
			printf("connect error:%d\n", WSAGetLastError());
			cleanConnect();
		}

		return listenSocket; 
	};

	int cleanConnect() {
		if(listenSocket != NULL )
			closesocket(listenSocket);

		if(clientSocket != NULL )
			closesocket(clientSocket);

		WSACleanup();
		return 0;
		};

};
