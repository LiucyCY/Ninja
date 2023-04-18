#pragma once
#ifndef IPC_H
#define IPC_H
#define BuffSize 1024
#define IpcAPI _declspec(dllexport)

#define BuffSize 1024
struct AppProtocol {
	char type[10];
	char end;
	int fileSize;
	char filePath[260];
	char data[BuffSize];
};

class IpcAPI Process {
public:
	static Process* createProcess();
	virtual int createNewProcess(const char*) = 0;
	virtual int exitProcess() = 0;
	virtual int initPipe() = 0;
	virtual int serverConnPipe() = 0;
	virtual int clientConnPipe() = 0;
	virtual int disConnPipe() = 0;

	virtual int wPipe(AppProtocol*) = 0;
	virtual int rPipe(AppProtocol*) = 0;
	virtual int closePipe() = 0;

	virtual int initAP(AppProtocol*) = 0;
	virtual int cptoi(char*) = 0;
	virtual int itocp(int, char*) = 0;
};

#endif
