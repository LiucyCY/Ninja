#pragma once
#include<string>
#include<iostream>
using namespace std;

typedef unsigned short WORD;
typedef unsigned char BYTE;

#define BuffSize 1024
#define MAKEWORD(low, high) \
	((WORD) ( ((BYTE)(low) )  | ( ((WORD)(BYTE)(high)) << 8 )))
// low ���ڵ�λ, high���ڸ�λ������λ����8 bit��

int IP = 0;
int PORT = 2000;
char PORT_STR[] = "2000";
char SERVER_IP[] = "127.0.0.1";

unsigned short socketVersion = MAKEWORD(1,1);
// MAKEWORD(1,1) ��ʾʹ��1.1�汾��socket
// MAKEWORD(2,1) ��ʾ2.1�汾��ͨ��ʹ��2.2�汾
/*
MAKEWORD( 1, 0) : 0000 0000 0000 0001 = 1 + 0 = 1
MAKEWORD( 1, 1) : 0000 0001 0000 0001 = 1 + 256 = 257
MAKEWORD( 2, 0) : 0000 0000 0000 0010 = 2 + 0 = 2
MAKEWORD( 2, 1) : 0000 0001 0000 0010 = 2 + 256 = 258
MAKEWORD( 2, 2) : 0000 0010 0000 0010 = 2 + 512 = 514
*/

// socket( int af, int type, int protocol);
// af
/*
#define AF_INET 2	// IPv4
#define AF_INET6 23	// IPv6
// type
#define SOCK_STREAM 1 // TCP
#define SOCK_DGRAM 2 // UDP
// protocol
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
*/

int af = 2;
int type = 1;
int protocol = 6; 

struct AppProtocol {
	char type[10];
	char end;
	int fileSize;
	char filePath[260];
	unsigned char data[BuffSize];
};

/*
�ܴ�С�� 10 + 1 + 4 + 1024 + 260 = 1299 Byte
Windows��ļ�·��Ϊ260���ַ�
type :
	cmd, upFile, dowFile, disconn, killPro
*/

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
