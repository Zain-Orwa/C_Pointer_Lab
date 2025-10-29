#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *name = (char *) malloc(strlen("Susan") + 1);
    strcpy(name, "Susan");

    while (*name != '\0') {
        printf("%c", *name);
        name++; // shifts pointer forward
    }
}
