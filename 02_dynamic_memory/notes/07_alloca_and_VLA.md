````markdown
# The alloca Function and Variable Length Arrays (VLAs)

---

## 1️⃣ Overview

The `alloca()` function provides a way to allocate memory **on the stack** rather than on the heap.  
Unlike `malloc()` or `calloc()`, which allocate memory from the heap and require manual `free()`,  
memory allocated by `alloca()` is **automatically freed** when the function that called it returns.

> ⚠️ Because it allocates memory from the stack, `alloca()` is **non-standard**,  
> not portable, and should generally be avoided in production code.

---

## 2️⃣ Function Prototype

```c
void *alloca(size_t size);
````

| Parameter | Type     | Description                               |
| :-------- | :------- | :---------------------------------------- |
| `size`    | `size_t` | Number of bytes to allocate on the stack. |

---

## 3️⃣ Behavior and Characteristics

| Property            | Description                                              |
| :------------------ | :------------------------------------------------------- |
| **Allocation Type** | Stack-based (inside the current function’s stack frame). |
| **Lifetime**        | Automatically freed when the function returns.           |
| **Deallocation**    | No `free()` required — freed automatically.              |
| **Portability**     | Non-standard; may not be available on all systems.       |
| **Use Case**        | Temporary storage for local use inside a function.       |

---

## 4️⃣ Example — Using `alloca()`

```c
#include <stdio.h>
#include <alloca.h>

void compute(int size) {
    char *buffer = alloca(size); // memory allocated on the stack
    for (int i = 0; i < size; i++)
        buffer[i] = 'A' + (i % 26);
    buffer[size - 1] = '\0';
    printf("Buffer: %s\n", buffer);
}

int main() {
    compute(10);
    compute(20);
    return 0;
}
```

✅ Output (example):

```
Buffer: ABCDEFGHI
Buffer: ABCDEFGHIJKLMNOPQRST
```

🧠 The key point:
When `compute()` ends, the stack frame (and its allocated memory) disappears automatically.

---

## 5️⃣ Important Notes

* Memory allocated with `alloca()` lives **only** inside the current function.
  You must **not return** a pointer to it, because once the function ends, the stack memory becomes invalid.
* If you need memory that outlives a function call, always use **heap allocation** (`malloc` / `calloc`).
* Using large allocations with `alloca()` can cause **stack overflow**.

---

## 6️⃣ Variable Length Arrays (VLAs)

In **C99**, Variable Length Arrays were introduced as a **standard** alternative to `alloca()`.
They allow the declaration of arrays whose size is determined at **runtime**.

Example:

```c
void compute(int size) {
    char buffer[size]; // VLA (stack-allocated)
    ...
}
```

✅ Here, the size of `buffer` depends on the parameter `size` — it’s not a constant.

| Feature            | Description                           |
| :----------------- | :------------------------------------ |
| **Storage**        | Stack                                 |
| **Lifetime**       | Ends when the function returns        |
| **Initialization** | Optional; behaves like a normal array |
| **Standard**       | Introduced in C99 (unlike `alloca()`) |
| **Deallocation**   | Automatic when function exits         |

---

## 7️⃣ Differences Between `alloca()` and VLAs

| Aspect              | `alloca()`                       | Variable Length Array (VLA)  |
| :------------------ | :------------------------------- | :--------------------------- |
| **Standardization** | Non-standard (compiler-specific) | Standard (C99)               |
| **Syntax**          | Function call                    | Normal array declaration     |
| **Deallocation**    | Automatic at function return     | Automatic at function return |
| **Lifetime**        | Same as function scope           | Same as function scope       |
| **Portability**     | Limited                          | High (C99 and later)         |
| **Use Case**        | Temporary dynamic buffer         | Runtime-sized local array    |

---

## 8️⃣ Notes About VLAs

* VLAs allocate memory **at runtime** just like `alloca()`, but **safely and portably**.
* Their size is **fixed once created** — you cannot resize a VLA after declaration.
  If you need a resizable array, use **heap allocation** with `realloc()`.
* The `sizeof` operator used with a VLA is evaluated **at runtime**, not compile time.

---

## 9️⃣ Example — Variable Length Array

```c
#include <stdio.h>

void compute(int size) {
    char buffer[size]; // VLA allocated on the stack
    for (int i = 0; i < size - 1; i++)
        buffer[i] = 'a' + (i % 26);
    buffer[size - 1] = '\0';
    printf("VLA buffer: %s\n", buffer);
}

int main(void) {
    compute(8);
    compute(12);
    return 0;
}
```

✅ Output:

```
VLA buffer: abcdefg
VLA buffer: abcdefghijk
```

---

## 🔟 Summary

| Concept            | Description                                                                              |
| :----------------- | :--------------------------------------------------------------------------------------- |
| **`alloca()`**     | Allocates memory on stack, automatically freed, non-portable.                            |
| **VLA (C99)**      | Stack-allocated array with runtime size; standard and safer.                             |
| **Deallocation**   | Automatic at end of function (no `free()`).                                              |
| **Limitation**     | Fixed size once declared — cannot use `realloc()`.                                       |
| **Recommendation** | Use VLA for small, short-lived local arrays; use heap (`malloc`) for dynamic structures. |

---

### ⚠️ Important Safety Rule

> Never return pointers to stack-allocated memory (from `alloca` or VLAs).
> Once the function ends, that memory ceases to exist — using it leads to **undefined behavior**.

---

📘 **Next Section:**
➡️ `08_using_free_function.md` — releasing dynamically allocated heap memory safely.

---

```
