#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS 1
#include"values.h"
#include<iostream>
#include<fstream>
#include<windows.h>
using namespace std;

class ioFile{
public:
	fstream file;
	ioFile() {};

	bool isFileExist(const char* path) { 
		fstream f = fstream(path);
		return f.good(); 
	}

	int writeFile(const char* path) { 
		if (isFileExist(path)) {
			file.open( path, ios::app);
			// file.write(recvBuff, strlen(recvBuff));
			file.close();
		}
		else {
			file.open(path, ios::out);
			// file.write(recvBuff, strlen(recvBuff));
			file.close();
		}
		return 0; 
	}

	int sendFile(const char* path) { return 0; }

	int pwd(char* filePath) { 
		GetModuleFileNameA(NULL,filePath, BuffSize); 
		(strrchr(filePath, '\\'))[0] = '\0';
		return 0;
	}
};
