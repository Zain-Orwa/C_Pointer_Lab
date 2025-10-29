````markdown
# 💾 Dynamic Memory Allocation in C

---

## 🧠 1️⃣ Concept Overview

Dynamic memory allocation allows a C program to request and release memory **at runtime**, instead of having fixed sizes at compile time.

The three basic steps for using dynamic memory are:

1. Use a `malloc`-type function to allocate memory  
2. Use that memory for your application’s data  
3. Release it when done using the `free` function

---

## ⚙️ 2️⃣ The Basic Workflow

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *pi = (int *) malloc(sizeof(int)); // allocate 4 bytes on heap
    *pi = 5;                               // store value in allocated memory
    printf("*pi: %d\n", *pi);              // print stored value
    free(pi);                              // release memory
    return 0;
}
````

🧾 **Output:**

```
*pi: 5
```

---

### 🧭 Memory Visualization

```text
┌──────────────┐
│   Stack       │
│  pi → 500     │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│   Heap        │
│ [ 5 ]  (addr 500)
└──────────────┘
```

💡 The variable `pi` (on the **stack**) stores the **address** of a memory block located on the **heap**.

---

## 📏 3️⃣ malloc() Function

The `malloc()` function allocates a specified number of bytes and returns a pointer to the beginning of the allocated block.

```c
void *malloc(size_t size);
```

* **Argument:** number of bytes to allocate
* **Return:** pointer to allocated memory (or `NULL` if allocation fails)

Example:

```c
int *pi = (int *) malloc(sizeof(int));
```

Equivalent to:

```c
int *pi = (int *) malloc(4);
```

…but using `sizeof()` is **more portable**, since the size of `int` can vary between systems.

---

## ⚠️ 4️⃣ Common Mistake — Dereferencing Before Assignment

🚫 **Incorrect:**

```c
int *pi;
*pi = (int *) malloc(sizeof(int)); // ❌ Wrong!
```

Here, `*pi` dereferences an **uninitialized pointer**, which points to garbage.
You are assigning the heap address to that garbage location → **undefined behavior**.

✅ **Correct:**

```c
int *pi = (int *) malloc(sizeof(int));
```

Always assign directly to the pointer, **not its dereferenced value**.

---

## 🧱 5️⃣ Using free() Function

Every successful allocation must be matched by a corresponding `free()` to avoid **memory leaks**.

```c
#include <stdlib.h>
free(pointer);
```

When you free memory:

* The **allocated block** is released back to the system
* The pointer still holds the same (now invalid) address
* Always set freed pointers to `NULL` after freeing

Example:

```c
free(pi);
pi = NULL;  // prevent dangling pointer
```

---

## 🧩 6️⃣ Heap Manager and Extra Memory

When you call `malloc()`, the **heap manager** may allocate **extra hidden memory** around your requested block to store management data (size, status, etc.).

This is invisible to your program but critical for internal bookkeeping.

### Example:

```c
char *pc = (char *) malloc(6);
for (int i = 0; i < 8; i++) {
    pc[i] = 0;
}
```

Here, memory was allocated for 6 bytes, but the loop writes **8 bytes** —
overflowing into heap manager metadata.

### Visualization

```text
┌──────────────────────────────────────────────┐
│ Heap:                                        │
│ [ 0 0 0 0 0 0 ][ Extra memory for metadata ] │
│ ↑                                             │
│ pc → first allocated byte                    │
└──────────────────────────────────────────────┘
```

💣 Writing beyond allocated space corrupts the heap → unpredictable behavior.

---

## 🧮 7️⃣ Key Rules of Dynamic Memory

| Rule # | Description                                                        |
| :----- | :----------------------------------------------------------------- |
| 1      | Always check if `malloc()` returns `NULL`.                         |
| 2      | Match every `malloc()` / `calloc()` / `realloc()` with a `free()`. |
| 3      | Never dereference an uninitialized or freed pointer.               |
| 4      | Use `sizeof()` for portability.                                    |
| 5      | Do not write beyond the allocated range.                           |
| 6      | After `free()`, set the pointer to `NULL`.                         |

---

## 🔍 8️⃣ Example — Allocation and Free Sequence

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *num = malloc(sizeof(int));
    if (num == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    *num = 42;
    printf("Stored value: %d\n", *num);

    free(num);
    num = NULL;

    return 0;
}
```

✅ Output:

```
Stored value: 42
```

---

## ⚖️ 9️⃣ Stack vs Heap Comparison

| Feature            | Stack (Automatic)                     | Heap (Dynamic)                                            |
| :----------------- | :------------------------------------ | :-------------------------------------------------------- |
| **Allocation**     | Done automatically by compiler        | Done manually using `malloc()` / `calloc()` / `realloc()` |
| **Deallocation**   | Automatic when function returns       | Must call `free()` manually                               |
| **Lifetime**       | Exists during function execution only | Persists until freed explicitly                           |
| **Speed**          | Very fast                             | Slower, system-managed                                    |
| **Typical Errors** | Stack overflow                        | Memory leak, dangling pointer, heap corruption            |

---

## 💬 🔟 Summary

> 🧠 `malloc()` allocates memory from the heap.
> 🧠 `free()` releases it — failing to do so causes **leaks**.
> 🧠 Use `sizeof()` to make allocations portable.
> 🧠 Never dereference an uninitialized or freed pointer.
> 🧠 Always pair allocation and deallocation in clean, balanced code.

---

```


