#include <stdlib.h>
#include <stdio.h>

const int a = 6;

char isel[] = { 'I', 'S', 'E', 'L', '\0' };

char const * const str1 = "ISEL"; // a const pointer to const chars
char const * str2  = "ISEL";      // a non-const pointer to const chars
const char * str2x = "ISEL";      // a non-const pointer to const chars
char * const str3 = isel;         // a const pointer to non-const chars
char * str4 = isel;               // a non-const pointer to non-const chars

int b = 8;

const int d = 10;

void func();

int main() {
	int c = 7;
	void * ptr = malloc(1);
	
	printf("[.rodata] %p\n", &a);
	printf("[.rodata] %p\n", &d);
	
	printf("[.data] %p\n", &b);

	printf("[local] %p\n", &c);
	printf("[local] %p\n", &ptr);
	
	printf("[dynamic] %p\n", ptr);

	printf("[function] %p\n", main);
	
	func();
	
	getchar();
	return 0;
}