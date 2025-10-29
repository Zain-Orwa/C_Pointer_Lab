````markdown
# 🧠 Memory Leaks in C

---

## 🔹 1️⃣ What Is a Memory Leak?

A **memory leak** occurs when memory is allocated from the heap but never freed, even though it is no longer used.

In other words, the program “forgets” about that memory — so it stays reserved until the program ends.

### Common Causes:
- The **address** of the allocated memory is lost.  
- The **`free()`** function is never called.  
- The program keeps objects in heap memory longer than needed (hidden leaks).

---

## 🔹 2️⃣ Why Memory Leaks Matter

When leaked, memory **cannot be reclaimed** or reused by the heap manager.  
Over time, this reduces available memory. In severe cases:
- The program runs out of memory (`malloc()` fails).
- The system slows down or crashes.

### Example:

```c
char *chunk;
while (1) {
    chunk = (char *) malloc(1000000); // 1 MB each iteration
    printf("Allocating\n");
}
````

🧾 **What happens:**

* Each loop allocates 1 MB.
* The previous address is overwritten before freeing it.
* The heap keeps growing until the system runs out of memory.

---

## 🔹 3️⃣ Losing the Address

Sometimes the address of a previously allocated block is overwritten before `free()` is called.

### Example:

```c
int *pi = (int *) malloc(sizeof(int));
*pi = 5;

/* Lost previous allocation! */
pi = (int *) malloc(sizeof(int));
```

💣 The first block is **lost** — no pointer refers to it anymore.
Even if you call `free(pi)` later, it only frees the **second** allocation.

---

### 🧭 Memory Visualization

```text
Before second malloc()
Stack:
  pi → 500
Heap:
  [500]: 5

After second malloc()
Stack:
  pi → 600
Heap:
  [500]: 5 (lost)
  [600]: ?? (new allocation)
```

| State                    | pi Address | Heap Block       | Problem     |
| :----------------------- | :--------- | :--------------- | :---------- |
| Before second `malloc()` | 500        | Holds `5`        | OK          |
| After second `malloc()`  | 600        | First block lost | Memory leak |

---

## 🔹 4️⃣ Example — Losing Address with String Pointer

```c
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
```

🧾 **Problem:**
At the end of the loop, `name` no longer points to the start of the string.
It now points to the string’s **null terminator**, so the original address is lost — you can’t `free(name)` anymore.

---

### 🧭 Memory Visualization

```text
Heap:
600 → [S][u][s][a][n][\0]
Stack:
name → 605  (now points at '\0')
```

| Step  | `name` Points To | Comment                                     |
| :---- | :--------------- | :------------------------------------------ |
| Start | `'S'`            | OK                                          |
| End   | `'\0'`           | Lost start address — cannot `free()` safely |

---

## 🔹 5️⃣ Hidden Memory Leaks

A **hidden leak** happens when:

* An object or structure remains allocated longer than needed.
* The programmer forgets to release its resources.

This doesn’t crash immediately but wastes memory quietly.

Example:
If you allocate memory inside a `struct` and only free the struct itself without freeing its inner pointers, those internal allocations remain leaked.

---

### Example Pattern

```c
struct person {
    char *name;
    int *scores;
};

struct person *p = malloc(sizeof(struct person));
p->name = malloc(50);
p->scores = malloc(10 * sizeof(int));

free(p); // ❌ Wrong — leaks p->name and p->scores
```

✅ **Correct Sequence:**

```c
free(p->name);
free(p->scores);
free(p);
```

---

## 🔹 6️⃣ Summary of Leak Types

| Leak Type        | Description                                                   | Example Cause                                   |
| :--------------- | :------------------------------------------------------------ | :---------------------------------------------- |
| **Lost Address** | The pointer is reassigned before freeing the original memory. | `p = malloc(...)` called twice without `free()` |
| **Never Freed**  | Allocated block not freed before program exits.               | Forgetting `free()` entirely                    |
| **Hidden Leak**  | Object stays in heap even when not needed.                    | Keeping unused data structures                  |
| **Nested Leak**  | Structure freed without freeing its dynamic members.          | Freeing outer struct only                       |

---

## 🔹 7️⃣ Prevention Strategies

| Strategy                                     | Description                                                  |
| :------------------------------------------- | :----------------------------------------------------------- |
| **1. Always call `free()`**                  | Pair every allocation with a proper release.                 |
| **2. Set pointers to `NULL` after freeing.** | Prevents dangling references.                                |
| **3. Use memory analysis tools.**            | Tools like *Valgrind*, *AddressSanitizer* help detect leaks. |
| **4. Track ownership clearly.**              | Decide which function is responsible for freeing memory.     |
| **5. Avoid unnecessary reallocations.**      | Reuse allocated memory where possible.                       |

---

## 💬 8️⃣ Key Takeaways

* Losing or forgetting a pointer → memory leak.
* A pointer moved inside an allocated block → cannot free safely.
* Free in reverse order of allocation when dealing with nested structures.
* Use tools and discipline to detect leaks early.
* Even small leaks add up — avoid them from day one.

---

```


