#include <stdio.h>
#include <stdlib.h>

int main(){
	int *pv = (int*)malloc(5 * sizeof(int));

	for (int i = 0; i < 5; i++){
		*(pv + i) = i + 1;
	}

	for (int i = 0; i < 5; i++){
		printf("pv[%d] = %d\n", i, *(pv + i));
	}

	free(pv);
	pv = NULL;

	return (0);
}
