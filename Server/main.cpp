#include"head.h"
#include<stdio.h>

int main() {
	int flag = 1;
	Interact inter = Interact();
	while (flag) {
		flag = inter.menu();

		switch (flag)
		{
		case 1:
			break;
		case 2:
			inter.console();
			break;
		default:
			break;
		}
	}
	return 0;
}