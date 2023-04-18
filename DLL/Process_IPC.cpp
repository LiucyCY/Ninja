#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include"ProcessIPC.h"
#include<string>
#include<iostream>
#include<windows.h>
using namespace std;

class process : public Process{
public:

	int flag = 0;
	AppProtocol *ap;
	process() {
		ap = (AppProtocol*)malloc(sizeof(AppProtocol));
		initAP(ap);
	};

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	unsigned long dwReadLen = 0, dwWriteLen = 0;

	int createNewProcess(const char* fileName) {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));
		unsigned long exitCode = -1;
		int flag = 0;
		flag = CreateProcess(
			fileName,
			NULL,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi
		);
		if (flag == 0) {
			cout << "create process error:" << GetLastError() << "\n";
			return -1;
		}
		return 0;
	};

	int exitProcess() { return 0; };
	
	int initPipe() { 
		hPipe = CreateNamedPipe("\\\\.\\pipe\\consolePipe",
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			0,
			1,
			BuffSize,
			BuffSize,
			0,
			NULL
		);

		if (hPipe == INVALID_HANDLE_VALUE) {
			cout << "server create Pipe error: " << GetLastError() << endl;
			hPipe = NULL;
			return -1;
		}

		return 0; 
	}

	int serverConnPipe() {
		if (flag == 1)
			return -1;
		if (!ConnectNamedPipe(hPipe, NULL)) {
			int flag = GetLastError();
			if (flag != ERROR_IO_PENDING) {
				cout << "server pipe connect error :" << GetLastError() << endl;
				hPipe = NULL;
			}
		}
		flag = 1;
		return 0;
	};

	int clientConnPipe() {

		if (!WaitNamedPipe("\\\\.\\pipe\\consolePipe", NMPWAIT_WAIT_FOREVER)) {
			cout << "client pipe wait failed : " << GetLastError() << endl;
		}

		hPipe = CreateFile("\\\\.\\pipe\\consolePipe",
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hPipe == INVALID_HANDLE_VALUE) {
			cout << "client create Pipe error: " << GetLastError() << endl;
			hPipe = NULL;
			return -1;
		}
		return 0;
	};

	int disConnPipe() {
		if (hPipe != INVALID_HANDLE_VALUE) {
			DisconnectNamedPipe(hPipe);
			flag = 0;
		}
		return 0;
	}

	int wPipe(AppProtocol *ap) {
		int flag = 0;
		char size[4];
		itocp(ap->fileSize, size);
		flag = WriteFile(hPipe, ap->type, 10, &dwWriteLen, NULL);
		if (flag == 0) {
			cout << "write pipe type error: " << GetLastError() << endl;
			return -1;
		}

		flag = WriteFile(hPipe, &ap->end, 1, &dwWriteLen, NULL);
		if (flag == 0) {
			cout << "write pipe end error: " << GetLastError() << endl;
			return -1;
		}

		flag = WriteFile(hPipe, size, 4, &dwWriteLen, NULL);
		if (flag == 0) {
			cout << "write pipe path error: " << GetLastError() << endl;
			return -1;
		}

		flag = WriteFile(hPipe, ap->filePath, 260, &dwWriteLen, NULL);
		if (flag == 0) {
			cout << "write pipe path error: " << GetLastError() << endl;
			return -1;
		}

		flag = WriteFile(hPipe, ap->data, 1024, &dwWriteLen, NULL);
		if (flag == 0) {
			cout << "write pipe path error: " << GetLastError() << endl;
			return -1;
		}
		return 0;
	}

	int rPipe(AppProtocol *ap) {
		initAP(ap);
		int flag = 0;
		char size[4];
		flag = ReadFile(hPipe, ap->type, 10, &dwReadLen, NULL);
		flag = ReadFile(hPipe, &ap->end, 1, &dwReadLen, NULL);
		flag = ReadFile(hPipe, size, 4, &dwReadLen, NULL);
		flag = ReadFile(hPipe, &ap->filePath, 260, &dwReadLen, NULL);
		flag = ReadFile(hPipe, &ap->data, BuffSize, &dwReadLen, NULL);
		ap->fileSize = cptoi(size);
		return 0;
	}

	int closePipe() {
		if (hPipe != NULL)
			CloseHandle(hPipe);
		return 0;
	}

	int initAP(AppProtocol *ap) {
		ap->end = 'n';
		ap->fileSize = 0;
		memset(ap->type, NULL, 10);
		memset(ap->filePath, NULL, 260);
		memset(ap->data, NULL, BuffSize);
		return 0;
	}

	int cptoi(char* s) {
		int a = atoi(s);
		return a;
	}

	int itocp(int a, char* s) {
		string str = to_string(a);
		s = (char*)str.c_str();
		return 0;
	}

};

Process* Process::createProcess(){
	return new process();
}
