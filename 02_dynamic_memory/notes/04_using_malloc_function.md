````markdown
# Using the malloc Function

---

## 1️⃣ Overview

The `malloc()` function allocates a block of memory from the **heap**.  
It takes one argument — the number of bytes to allocate — and returns a **pointer to void** (`void *`).

If there isn’t enough memory available, `malloc()` returns `NULL`.

The contents of the allocated memory are **not initialized**, meaning they contain **garbage values** and must be explicitly set by the program before use.

---

## 2️⃣ Function Prototype

```c
void *malloc(size_t size);
````

| Parameter | Type     | Description                                |
| :-------- | :------- | :----------------------------------------- |
| `size`    | `size_t` | Number of bytes to allocate from the heap. |

---

## 3️⃣ Key Characteristics

| Property           | Description                                               |
| :----------------- | :-------------------------------------------------------- |
| **Location**       | Allocates memory on the **heap**, not the stack.          |
| **Return type**    | Returns a pointer of type `void *`.                       |
| **Initialization** | Memory is **not cleared** — contains random garbage data. |
| **Failure**        | Returns `NULL` if allocation fails.                       |
| **Header**         | Declared in `<stdlib.h>`.                                 |

---

## 4️⃣ Typical Example

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *pi = (int *) malloc(sizeof(int)); // allocate 4 bytes
    if (pi != NULL) {
        *pi = 5;
        printf("*pi = %d\n", *pi);
        free(pi);
    } else {
        printf("Memory allocation failed!\n");
    }
    return 0;
}
```

### Execution Steps:

1. Memory is allocated from the heap.
2. Memory is **not modified** or cleared.
3. A pointer to the **first byte** of the allocated block is returned.

---

## 5️⃣ Handling malloc() Failures

Since `malloc()` can return `NULL`, always test the result before dereferencing the pointer.

```c
int *pi = (int *) malloc(sizeof(int));
if (pi == NULL) {
    printf("Memory allocation failed.\n");
    return 1;
}
```

Failing to check for `NULL` may cause **segmentation faults** if you later dereference an invalid pointer.

---

## 6️⃣ Casting malloc()

Explicit casting of `malloc()`’s return value is **not required in C**, because `void *` is automatically converted to any other pointer type.
However, it’s sometimes used for clarity or C++ compatibility.

```c
int *pi = (int *) malloc(sizeof(int)); // explicit cast (optional)
int *pj = malloc(sizeof(int));         // no cast (valid in C)
```

### When to Cast:

| Reason                | Explanation                                          |
| :-------------------- | :--------------------------------------------------- |
| **Documentation**     | Makes the type of allocated memory clear to readers. |
| **C++ Compatibility** | C++ requires explicit casts from `void *`.           |

### When Not to Cast:

* If you **forget to include `<stdlib.h>`**, the compiler assumes `malloc()` returns an `int`.
  The explicit cast can **hide this mistake** and cause runtime issues.

---

## 7️⃣ Common Mistake — Using malloc Without Allocation

Failing to allocate memory before dereferencing a pointer leads to undefined behavior.

```c
int *pi;
printf("%d\n", *pi); // ❌ pi points to random memory
```

This usually causes a **segmentation fault**, because `pi` has not been initialized or assigned any valid address.

### Correct Approach:

```c
int *pi = malloc(sizeof(int));
if (pi != NULL) {
    *pi = 10;
    printf("%d\n", *pi);
    free(pi);
}
```

---

## 8️⃣ Common Mistake — Using malloc() With Strings Without Allocation

```c
char *name;
printf("Enter a name: ");
scanf("%s", name); // ❌ no memory allocated for name
```

Even though it looks valid, `name` points to an unknown location.
You must allocate memory before storing data in it.

### Correct Version:

```c
char *name = malloc(20 * sizeof(char));
printf("Enter a name: ");
scanf("%19s", name);
printf("Hello, %s\n", name);
free(name);
```

---

## 9️⃣ Using sizeof() Correctly

Always use `sizeof()` when calculating the number of bytes to allocate.
This ensures your program is **portable** across architectures with different type sizes.

```c
double *pd = (double *) malloc(10 * sizeof(double)); // allocates space for 10 doubles
```

If you forget `sizeof()`, only a few bytes may be allocated:

```c
double *pd = (double *) malloc(10); // ❌ allocates only 10 bytes, not 10 doubles
```

---

## 🔟 Determining the Amount of Memory Allocated

There is **no standard way** to determine the exact size of a block allocated by `malloc()`.
Some compilers offer extensions, but in general, you must **track the size yourself**.

Keep a variable that records the number of allocated elements or bytes.

Example:

```c
size_t size = 100;
int *data = malloc(size * sizeof(int));
if (data == NULL) exit(1);
```

---

## 11️⃣ Heap Manager Overhead

The heap manager may allocate extra bytes around your block for **metadata** (like block size and state).
This overhead is invisible to you but necessary for heap bookkeeping.

Example:
If you allocate 64 bytes, the heap might internally allocate 72 or 80 bytes, depending on implementation.

---

## 12️⃣ System and Implementation Limits

The maximum allocatable size depends on:

* The **amount of physical memory** or **address space** available.
* The **operating system** and its **runtime model**.
* The **implementation of malloc()** (some use lazy allocation).

---

## 13️⃣ Using malloc() With Static or Global Pointers

You **cannot** call `malloc()` as part of a static or global initialization.

```c
static int *pi = malloc(sizeof(int)); // ❌ not allowed
```

This causes a **compile-time error**, because static/global variables must be initialized at compile time, not at runtime.

### Correct Approach:

```c
static int *pi;  // declaration only
pi = malloc(sizeof(int)); // runtime assignment (valid)
```

The assignment must occur **inside a function**, since executable statements can only run after the program starts.

---

## 14️⃣ Summary Table

| Topic                 | Key Rule                                           |
| :-------------------- | :------------------------------------------------- |
| **Header**            | Include `<stdlib.h>` for malloc().                 |
| **Return Value**      | Returns `void *` (cast optional).                  |
| **Initialization**    | Memory is uninitialized (garbage values).          |
| **Error Handling**    | Always check for `NULL`.                           |
| **sizeof Usage**      | Always use `sizeof()` for portable allocation.     |
| **Static/Global Use** | Only assign inside a function, not at declaration. |

---

## 15️⃣ Summary

* `malloc()` allocates **uninitialized** memory on the **heap**.
* The pointer returned must be checked for `NULL` before use.
* Forgetting `sizeof()` or dereferencing an uninitialized pointer leads to **runtime errors**.
* You can’t initialize global/static pointers using `malloc()` directly.
* Always `free()` the memory when done.

---

📘 **Figures Referenced:**

* *Figure 2-5:* Failure to allocate memory

📚 **Next Section:**
➡️ `05_using_calloc_function.md` — how to allocate and automatically zero memory.

```

