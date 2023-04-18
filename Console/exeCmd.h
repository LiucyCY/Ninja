#pragma once
#include"ProcessIPC.h"
#include<windows.h>
#include<iostream>
#include<string>
#include<vector>
#define _CRT_SECURE_NO_WARNINGS 1
#define BuffSize 1024
#pragma warning(disable:4996)
#pragma comment(lib, "ProcessIPC.lib")

using namespace std;

class exeCmd {
public:
	Process *proc = Process::createProcess();
	AppProtocol *ap;

	exeCmd() { ap = (AppProtocol*)malloc(sizeof(AppProtocol)); proc->initAP(ap); }

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
		vector<string> result = split(cmd, " ");
		const char *type = "cmd";
		const char *command = "";
		if (result[0] == "ls") {
			command = "dir";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
		}
		else if (result[0] == "pwd") {
			command = "chdir";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
		}
		else if (result[0] == "exit") {
			type = "exit";
			command = "exit";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
			return -1;
		}
		else if(result[0] == "sendFile"){
			cout << "执行新进程\n";

			command = "chdir";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
		}
		else if(result[0] == "downFile"){
			cout << "执行新进程\n";

			command = "chdir";
			memcpy(ap->type, type, strlen(type));
			memcpy(ap->data, command, strlen(command));
			proc->wPipe(ap);
		}
		else {
			return 0;
		}
		return 0;
	}

	vector<string> split(string cmd, string c) {
		vector<string> result;
		int flag = 0;
		while (flag != cmd.npos) {
			flag = cmd.find_first_of(c);
			if (flag > 0)
				result.push_back(cmd.substr(0, flag));
			cmd = cmd.substr(flag+1);
		}
		if (cmd.length() > 0)
			result.push_back(cmd);
		return result;
	}

};
