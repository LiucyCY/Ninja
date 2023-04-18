#pragma once
#pragma warning(disable:4996)
#include"values.h"
#include"connect.h"
#include"transfer.h"

class Control {
public:
	Connect c;
	SOCKET clientSocket;

	SOCKET connect() {
		c.initConnect();
		clientSocket = c.createConnect();
		if (clientSocket == INVALID_SOCKET) {
			cout << "connect error:%d\n"<< WSAGetLastError();
			return -1;
		}
		cout << "connect success!\n";
		return clientSocket;
	}

	int disconnect() {
		c.cleanConnect();
		cout << "stop connect\n";
		return 0;
	}
};
