#pragma once
#include"ProcessIPC.h"
#include<windows.h>
#include<iostream>
#include<string>
#define _CRT_SECURE_NO_WARNINGS 1
#define BuffSize 1024
#pragma warning(disable:4996)
#pragma comment(lib, "ProcessIPC.lib")

using namespace std;

class exeCmd {
public:
	Process *proc = Process::createProcess();
	AppProtocol *ap;

	exeCmd() { ap = (AppProtocol*)malloc(sizeof(AppProtocol)); initAP(ap); }

	int consoleUI() { 
		int flag = -1;
		string cmd;
		while (true) {
			proc->clientConnPipe();
			cout << "Ninja >>";
			getline(cin, cmd);
			flag = parsing(cmd);
			if (flag == -1) break;
			Sleep(100);
			while (true) {
				proc->rPipe(ap);
				if (ap->end == 'y')
					break;
				cout << ap->data;
			}
			proc->disConnPipe();
		};

		return 0; 
	}

	int parsing(string cmd) {
		string s;
		const char *type = "cmd";
		const char *command = "";
		if (cmd == "ls") {
			command = "dir";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
		}
		else if ( cmd == "poweroff" ) {
			s = "shutdown";
		}
		else if ( cmd == "whoami" ) {
			s = "Are you root?";
		}
		else if (cmd == "pwd") {
			command = "chdir";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
		}
		else if (cmd == "exit") {
			type = "exit";
			command = "exit";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
			return -1;
		}
		else {
			return 0;
		}
		return 0;
	}

};
