#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include"values.h"
#include"connect.h"
#include<stdio.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Connect {
public:
	int sockErr = 0;
	unsigned short wVersionRequested = socketVersion;
	WSADATA stWSAData;
	SOCKET listenSocket, clientSocket;
	addrinfo hints, *result = NULL;
	unsigned long iMode = 1; // 1为非阻塞， 0为阻塞

	int initConnect() { 
		sockErr = WSAStartup( wVersionRequested, &stWSAData);
		if (sockErr != 0) {
			unsigned short version = stWSAData.wVersion;
			unsigned short highest = stWSAData.wHighVersion;
			printf("version: %hu\nhighestVersion: %hu\n", version, highest);
			printf("sockErr: %d\n", sockErr);
		}

		hints.ai_family = af;
		hints.ai_socktype = type;
		hints.ai_protocol = protocol;
		hints.ai_flags = AI_PASSIVE;
		// 为后面的bind()准备数据
		int iResult = getaddrinfo(NULL, PORT_STR, &hints, &result); 
		if (iResult != 0) {
			printf("getaddrinfo is error:%d\n", iResult);
			cleanConnect();
		}

		listenSocket = socket(af, type, protocol);
		if (listenSocket == INVALID_SOCKET) {
			printf("LastError:%d\n", WSAGetLastError());
			cleanConnect();
		}

		iResult = bind( listenSocket, result->ai_addr, result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind error:%d\n", WSAGetLastError());
			cleanConnect();
		}

		iResult = listen( listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen error:%d\n", WSAGetLastError());
			cleanConnect();
		}

		return 0;
	};

	SOCKET createConnect() { 
		printf("waiting connect ....\n");
		clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept error:%d\n", WSAGetLastError());
			cleanConnect();
		}

		return clientSocket; 
	};

	int cleanConnect() {
		if (result != NULL)
			freeaddrinfo(result);

		if(listenSocket != NULL )
			closesocket(listenSocket);

		if(clientSocket != NULL )
			closesocket(clientSocket);

		WSACleanup();
		return 0;
		};

};
