#include <stdio.h>
#include <stdlib.h>

int global_var = 10;

int* make_stack_pointer(void) {
    int local_var = 20;
    return &local_var; // ❌ Dangling pointer after return
}

int main(void) {
    static int static_var = 30;
    int *heap_ptr = malloc(sizeof(int));
    *heap_ptr = 40;

    printf("&global_var = %p\n", (void*)&global_var);
    printf("&static_var = %p\n", (void*)&static_var);
    printf("heap_ptr (heap) = %p\n", (void*)heap_ptr);
    printf("make_stack_pointer() = %p (invalid after return!)\n",
           (void*)make_stack_pointer());

    free(heap_ptr);
    return 0;
}
