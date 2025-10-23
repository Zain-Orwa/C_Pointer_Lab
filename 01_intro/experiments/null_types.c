#include <stdio.h>
#include <stddef.h>


int main(void){
    int *p1 = 0;
    int *p2 = NULL;
    int *p3 = (void*)0;

    printf("p1=%p | p2=%p | p3=%p", (void*)p1, (void*)p2, (void*)p3);
    printf("All equal? %s\n", (p1 == p2 && p2 == p3) ? "YES" : "NO");

    return (0);
}

