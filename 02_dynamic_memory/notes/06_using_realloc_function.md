````markdown
# Using the realloc Function

---

## 1️⃣ Overview

Sometimes a program needs to **resize an existing memory block** — to expand it or shrink it — without losing the data it already holds.  
The `realloc()` function does exactly that.

---

## 2️⃣ Function Prototype

```c
void *realloc(void *ptr, size_t size);
````

| Parameter | Type     | Description                                                                   |
| :-------- | :------- | :---------------------------------------------------------------------------- |
| `ptr`     | `void *` | Pointer to the previously allocated block (`malloc`, `calloc`, or `realloc`). |
| `size`    | `size_t` | New requested size (in bytes).                                                |

---

## 3️⃣ Behavior Summary

| First Parameter | Second Parameter      | Behavior                                                                                    |
| :-------------- | :-------------------- | :------------------------------------------------------------------------------------------ |
| `NULL`          | N/A                   | Acts like `malloc(size)` — allocates new memory.                                            |
| Not `NULL`      | `0`                   | Frees the original block (same as calling `free(ptr)`).                                     |
| Not `NULL`      | Smaller than old size | Shrinks the block; extra memory may be released back to heap.                               |
| Not `NULL`      | Larger than old size  | Expands the block; data is preserved, and a new block may be allocated elsewhere if needed. |

---

## 4️⃣ Core Concept

* `realloc()` tries to **resize the block at the same address** if possible.
* If not possible (e.g., space unavailable), it:

  1. Allocates a **new block** of the requested size.
  2. Copies the contents of the old block into the new one.
  3. Frees the old block automatically.
  4. Returns a pointer to the new block.

⚠️ If reallocation fails, it returns `NULL`, **but the original memory remains valid**.

---

## 5️⃣ Example 1 — Shrinking a Block

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *string1, *string2;

    string1 = (char *) malloc(16);
    strcpy(string1, "0123456789AB");

    // shrink memory from 16 bytes → 8 bytes
    string2 = realloc(string1, 8);

    printf("string1 Value: %p [%s]\n", string1, string1);
    printf("string2 Value: %p [%s]\n", string2, string2);

    free(string2);
    return 0;
}
```

### Possible Output

```
string1 Value: 0x500 [0123456789AB]
string2 Value: 0x500 [0123456789AB]
```

✅ In this case, `realloc()` reused the same block — no copy occurred.
The heap manager just marked the unused bytes as free.

> ⚠️ However, since the string still uses more bytes than reallocated (8), this is unsafe.
> Always ensure the data fits the new size by adding a null terminator when shrinking strings.

---

## 🧩 Figure 2-6 — Shrinking Memory Block

```
Before:
Heap: [0123456789AB0...]  ← 16 bytes
After realloc(8):
Heap: [01234567]          ← 8 bytes used
```

---

## 6️⃣ Example 2 — Expanding a Block

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *string1, *string2;

    string1 = (char *) malloc(16);
    strcpy(string1, "0123456789AB");

    // enlarge memory from 16 bytes → 64 bytes
    string2 = realloc(string1, 64);

    printf("string1 Value: %p [%s]\n", string1, string1);
    printf("string2 Value: %p [%s]\n", string2, string2);

    free(string2);
    return 0;
}
```

### Possible Output

```
string1 Value: 0x500 [0123456789AB]
string2 Value: 0x600 [0123456789AB]
```

✅ The heap manager couldn’t extend the existing block,
so it allocated a new one (at 0x600), copied the data, and freed the old one.

---

## 🧠 Figure 2-7 — Expanding Memory Block

```
Before realloc():
Heap: [0123456789AB]      ← Address 0x500
After realloc(64):
Heap: [0123456789AB......]← Address 0x600 (new block)
```

---

## 7️⃣ Important Notes and Edge Cases

| Situation             | Behavior                                            |
| :-------------------- | :-------------------------------------------------- |
| `realloc(NULL, size)` | Same as `malloc(size)`                              |
| `realloc(ptr, 0)`     | Frees memory and returns `NULL`                     |
| If allocation fails   | Returns `NULL`, but old block remains valid         |
| When enlarging        | Contents are preserved, new memory is uninitialized |
| When shrinking        | Data beyond new size is lost or undefined           |

---

## 8️⃣ Error Handling Example

```c
int *arr = malloc(5 * sizeof(int));
if (!arr) return 1;

int *temp = realloc(arr, 10 * sizeof(int));
if (!temp) {
    printf("Reallocation failed.\n");
    free(arr);
    return 1;
}
arr = temp; // only assign if realloc succeeded
```

✅ Always store the result of `realloc()` in a **temporary pointer**.
If `realloc` fails and returns `NULL`, assigning it directly would lose access to the original memory — causing a **memory leak**.

---

## 9️⃣ Common Mistakes to Avoid

| Mistake                                    | Explanation                          |
| :----------------------------------------- | :----------------------------------- |
| Directly assigning result to same pointer  | May lose access if allocation fails  |
| Assuming data beyond new size is preserved | It’s not — only up to new size       |
| Using old pointer after `realloc`          | Old pointer becomes invalid if moved |
| Forgetting to `free()` after use           | Causes memory leaks                  |

---

## 🔟 Summary

| Topic                | Key Idea                                  |
| :------------------- | :---------------------------------------- |
| **Purpose**          | Resize dynamically allocated memory       |
| **Preserves Data**   | Yes (up to smaller of old/new size)       |
| **May Move Block**   | Yes, if necessary                         |
| **Fails Gracefully** | Returns `NULL`, keeps old block intact    |
| **Common Use Case**  | Expanding arrays or buffers as data grows |

---

### 🧭 Best Practice Rule

> Always treat `realloc()` like a “may move” operation —
> never assume the pointer stays the same, and never overwrite the old pointer until you’re sure it worked.

---


```

