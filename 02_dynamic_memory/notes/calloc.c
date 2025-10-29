#include <stdio.h>
#include <stdlib.h>

int main(){
	int *pi = calloc(5, sizeof(int));

	if (pi == NULL){
		printf("Memory allocation failed\n");
		return (1);
	}

	for (int i = 0; i < 5; i++){
		printf("pi[%d] = %d\n", i, *(pi + i)); // or instead of [*(pi + i)] pointer arith we can use [(pi[i])]//
	}

	return (0);
}
