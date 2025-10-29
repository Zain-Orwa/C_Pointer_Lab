#include <stdio.h>
#include <stdlib.h>

int main(){
  char *Chunk;

  while(1){
    Chunk = (char*)malloc(1000000);
    printf("Allocating\n");
  }

  printf("%c", *Chunk);

  return (0);
}
