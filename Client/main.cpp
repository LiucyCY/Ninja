#include<stdio.h>
#include"head.h"
#include<stdlib.h>

int main() {
	int flag = 1;
	Control c = Control();
	while (true) {
		flag = 1;
		while(flag)
			flag = c.connect();

		flag = 1;
		while(flag)
			flag = c.chat();

		c.disConnect();
	}
	return 0;
}