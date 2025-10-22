#include <stdio.h>
int main(void){
    int x = 42;
    int *p = &x;
    printf("x=%d  &x=%p  p=%p  *p=%d\n", x, (void*)&x, (void*)p, *p);
    return 0;
}
