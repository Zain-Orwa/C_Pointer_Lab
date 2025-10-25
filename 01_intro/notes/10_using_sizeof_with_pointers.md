````markdown
# 🧩 Using the `sizeof` Operator with Pointers

---

## 🧠 1️⃣ Overview

The `sizeof` operator can also be used to determine **how many bytes a pointer occupies** in memory.

Example:

```c
#include <stdio.h>

int main(void) {
    printf("Size of *char : %zu\n", sizeof(char *));
    printf("Size of *int  : %zu\n", sizeof(int *));
    printf("Size of *void : %zu\n", sizeof(void *));
    printf("Size of *func : %zu\n", sizeof(void (*)(void)));
    return 0;
}
````

### 🖥️ Example Output (32-bit system)

```
Size of *char : 4
Size of *int  : 4
Size of *void : 4
Size of *func : 4
```

✅ All pointer types typically have the **same size** on a given platform,
but that size **varies between systems** and **can differ between data and function pointers**.

---

## ⚙️ 2️⃣ Why You Should Use `sizeof`

Pointer size depends on:

* **CPU architecture** (32-bit vs 64-bit)
* **Compiler** and its options
* **Memory model** (flat vs segmented)
* **Instruction set** (Von Neumann vs Harvard architecture)

💡 Therefore:

> Always use `sizeof(type*)` to determine pointer size instead of assuming it.

---

## 🧮 3️⃣ Typical Pointer Sizes by Platform

| Platform / Architecture | Pointer Size | Notes                                          |
| :---------------------- | :----------: | :--------------------------------------------- |
| 16-bit microcontroller  |    2 bytes   | old embedded systems                           |
| 32-bit system           |    4 bytes   | common for legacy PCs                          |
| 64-bit system           |    8 bytes   | modern OS and CPUs                             |
| Harvard architecture    |   1–4 bytes  | data and code may have different pointer sizes |

---

## 🧩 4️⃣ Example — Printing Pointer Sizes

```c
#include <stdio.h>

int main(void) {
    printf("sizeof(char*)  = %zu bytes\n", sizeof(char *));
    printf("sizeof(int*)   = %zu bytes\n", sizeof(int *));
    printf("sizeof(double*)= %zu bytes\n", sizeof(double *));
    printf("sizeof(void*)  = %zu bytes\n", sizeof(void *));
    printf("sizeof(void(*)(void)) = %zu bytes\n", sizeof(void (*)(void)));
}
```

### 🖥️ Example Output (64-bit)

```
sizeof(char*)  = 8 bytes
sizeof(int*)   = 8 bytes
sizeof(double*)= 8 bytes
sizeof(void*)  = 8 bytes
sizeof(void(*)(void)) = 8 bytes
```

✅ On most 64-bit systems, all pointers are 8 bytes.
But function pointers **may differ** on unusual systems.

---

## 🧠 5️⃣ Harvard Architecture Notes

In a **Von Neumann architecture**, both **code** and **data** share the same physical memory.
Thus, data and function pointers have the same size.

In a **Harvard architecture**, **code and data reside in separate memory spaces**.
That means pointers to code and pointers to data may have **different sizes**.

### Example: Intel MCS-51 (8051)

* Code memory and data memory are **separate**.
* Pointers can vary from **1 to 4 bytes**, depending on the compiler and memory model.
* The **Small Device C Compiler (SDCC)** supports these architectures.

💡 On such systems:

> Always check pointer size with `sizeof()` — it’s not guaranteed to be consistent.

---

## 🧱 6️⃣ Summary Table

| Pointer Type     | Description              |     Typical Size (64-bit)    | Example             |
| :--------------- | :----------------------- | :--------------------------: | :------------------ |
| `char *`         | Points to character data |            8 bytes           | `char *p;`          |
| `int *`          | Points to integer data   |            8 bytes           | `int *p;`           |
| `void *`         | Generic data pointer     |            8 bytes           | `void *p;`          |
| `double *`       | Points to double         |            8 bytes           | `double *p;`        |
| `void (*)(void)` | Function pointer         | 8 bytes (but not guaranteed) | `void (*fp)(void);` |

---

## ⚠️ 7️⃣ Key Takeaways

> 🧠 The `sizeof` operator tells you **how many bytes a pointer consumes**, not what it points to.
> 🧠 Don’t assume pointer sizes — they depend on architecture and compiler.
> 🧠 Function pointers can differ in size from data pointers.
> 🧠 Embedded and Harvard architectures often use variable-length pointers.
> 🧠 Always print pointer sizes using `%zu` (unsigned type).

---

## 🧩 8️⃣ Visual Diagram — Conceptual Memory Map

```text
╔══════════════════════════════════════════════════════╗
║                Von Neumann Architecture              ║
║──────────────────────────────────────────────────────║
║ [ CODE + DATA in same memory space ]                 ║
║ → Function ptrs and data ptrs = same size            ║
╚══════════════════════════════════════════════════════╝

╔══════════════════════════════════════════════════════╗
║                 Harvard Architecture                 ║
║──────────────────────────────────────────────────────║
║ [ CODE memory separate from DATA memory ]            ║
║ → Code pointers may use different address width       ║
║ → Data pointers can be shorter or longer              ║
╚══════════════════════════════════════════════════════╝
```

---

## 🧪 9️⃣ Live Demo (runnable C)

```c
#include <stdio.h>

int main(void) {
    printf("Pointer sizes on this system:\n");
    printf("  sizeof(void*)        = %zu bytes\n", sizeof(void *));
    printf("  sizeof(char*)        = %zu bytes\n", sizeof(char *));
    printf("  sizeof(int*)         = %zu bytes\n", sizeof(int *));
    printf("  sizeof(double*)      = %zu bytes\n", sizeof(double *));
    printf("  sizeof(void (*)(void)) = %zu bytes\n", sizeof(void (*)(void)));

    int x = 42;
    int *px = &x;
    printf("\nAddress of x: %p, pointer variable px occupies %zu bytes.\n",
           (void*)px, sizeof(px));
    return 0;
}
```

### Example Output

```
Pointer sizes on this system:
  sizeof(void*)        = 8 bytes
  sizeof(char*)        = 8 bytes
  sizeof(int*)         = 8 bytes
  sizeof(double*)      = 8 bytes
  sizeof(void (*)(void)) = 8 bytes

Address of x: 0x7ffee4bfa1cc, pointer variable px occupies 8 bytes.
```

---

💬 **In short:**

> 🧩 `sizeof(pointer)` = number of bytes used to store a memory address.
> 🧩 Pointer size depends on **CPU architecture**, **compiler**, and **memory model**.
> 🧩 Always use `sizeof(type*)` instead of assuming fixed values like 4 or 8.

```

