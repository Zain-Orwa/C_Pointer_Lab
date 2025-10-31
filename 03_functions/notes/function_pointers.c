#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


bool freez_C(int temerature){
	if (temerature <= 0) return true;
	else return false;
}

bool freez_F(int temerature){
	if (temerature <= 32) return true;
	else return false;
}

void is_freezing(bool (*freez_check)(int) ){
	int temerature = 0;

	printf("Enter temerature: ");
	scanf("%d", &temerature);

	if (freez_check(temerature))
		printf("It's Freezing!\n");
	else
		printf("It's not Freezing!\n");
}


int main(void){
	printf("\nCelsius...\n\n");
	is_freezing(freez_C);
	is_freezing(freez_C);

	printf("\nFahrenheit...\n\n");
		is_freezing(freez_F);
		is_freezing(freez_F);
	
	return (0);
}
