#include <stdio.h>

int main(){

	short s;
	short *ps = &s;

	char c;
	char *pc = &c;

	printf("Content of ps before: %p\n", ps);
	ps = ps + 1;
	printf("Content of ps after: %p\n", ps);

	printf("Content of pc before: %p\n", pc);
	pc = pc + 1;
	printf("Content of pc after: %p\n", pc);


	return (0);
}



