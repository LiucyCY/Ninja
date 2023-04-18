#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include"values.h"
#include"control.h"
#include"process.h"
#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<string>
using namespace std;

class Interact {
public:
	SOCKET clientSocket = INVALID_SOCKET;
	Transfer trans;
	ioFile file;
	Process proc;
	struct AppProtocol *ap;

	Interact() { ap = (AppProtocol*)malloc(sizeof(AppProtocol)); }

	int menu() {
		const char *s = (char*)malloc(10*sizeof(char));
		int choice = 0;
		Control control;

		cout << "\n"
			<< "Ninja\n"
			<< "1. connect host\n"
			<< "2. console\n"
			<< "3. disconnect\n"
			<< "4. exit\n"
			<< "please choice:";
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			if (clientSocket != INVALID_SOCKET)
				cout << "you have already connected!\n";
			else {
				clientSocket = control.connect();
				trans = Transfer(clientSocket);
			}
			if (clientSocket == INVALID_SOCKET)
				cout << "connect error:" << WSAGetLastError() << "\n";
			return 1;
		case 2:
			if (clientSocket == INVALID_SOCKET) {
				cout << "you haven't connect\n";
				return 1;
			}
			else
				return 2;
		case 3:
			if (clientSocket == INVALID_SOCKET)
				cout << "you haven't connect\n";
			else
				control.disconnect();
			return 1;
		case 4:
			s = "discon";
			memcpy(ap->type, s, sizeof(s));
			trans.sendData(ap);
			if (clientSocket != INVALID_SOCKET)
				control.disconnect();
			cout << "have a nice day!\nBye\n";
			return 0;
		default:
			cout << "\ninput error!\n";
			return 1;
		}
		return 1;
	}

	int console() {
		char filePath[260];
		int flag = 1;
		proc.initPipe();
		file.pwd(filePath);
		strcat(filePath, "\\program\\ninjaConsole.exe");
		proc.createNewProcess(filePath);

		while (flag > 0) {
			proc.serverConnPipe();
			proc.rPipe(ap);
			flag = trans.parProtocol(ap);
			switch (flag)
			{
			case -1:
				proc.disConnPipe();
				break;
			case 1:
				// 执行命令函数
				interClientCmd(ap);
				break;
			case 2:
				// 上传文件函数
				break;
			case 3:
				// 下载文件函数
				break;
			default:
				break;
			}
		}
		proc.closePipe();
		return 0;
	}

	int interClientCmd(AppProtocol *ap) {
		trans.sendData(ap);
		while (true) {
			trans.recvData(ap);
			if (ap->end == 'y')	break;
			proc.wPipe(ap);
		}
		proc.wPipe(ap);
		proc.disConnPipe();
		return 0;
	}

};
