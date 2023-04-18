#pragma once
#pragma warning(disable:4996)
#include"values.h"
#include"transfer.h"
#include<stdio.h>
#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

class Parsing {
public:
	SOCKET listenSocket = INVALID_SOCKET;

	Parsing() {};
	Parsing(SOCKET listenSocket) { this->listenSocket = listenSocket; }
	
	int check() {
		if (listenSocket != INVALID_SOCKET)
			return 0;
		else
			return -1;
	}

	int parsingBuff(AppProtocol *ap) { 
		int flag = check();
		if (flag == -1)
			return -1;
		string cmd = ap->type;
		if (cmd == "cmd") {
			return 1;
		}
		else {
			return -1;
		}
		return 0; 
	}
};
