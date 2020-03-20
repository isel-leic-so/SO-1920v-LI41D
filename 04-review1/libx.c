#include <stdio.h>

const int x = 5;

int y = 9;

void func() {
	int z = 7;
	
	printf("[libx] [.rodata]  %p\n", &x);
	printf("[libx] [.data]    %p\n", &y);
	printf("[libx] [local]    %p\n", &z);
	printf("[libx] [function] %p\n", func);
}
