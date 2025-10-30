````markdown
# 🧹 Deallocating Memory Using the `free()` Function

---

## 1️⃣ Overview

When you allocate memory dynamically using `malloc()`, `calloc()`, or `realloc()`,  
that memory **must eventually be released** — otherwise, it remains reserved and can cause **memory leaks**.

The function responsible for releasing that memory is `free()`.

---

## 2️⃣ Function Prototype

```c
void free(void *ptr);
````

| Parameter | Type     | Description                                       |
| :-------- | :------- | :------------------------------------------------ |
| `ptr`     | `void *` | Pointer to the previously allocated memory block. |

---

## 3️⃣ How It Works

* The `free()` function **returns** a block of memory to the **heap**.
* Once freed, the pointer still *holds the same address*, but that memory is no longer valid to access.
* The region may later be **reused** for another allocation.

Example:

```c
int *pi = (int *) malloc(sizeof(int));
*pi = 5;
free(pi);
```

After `free(pi);`,

* The memory at address `pi` is released.
* The variable `pi` still holds the address (a “dangling pointer”).
* Accessing `*pi` after freeing is **undefined behavior**.

---

## 🧩 Figure 2-8 — Release of Memory Using `free`

```
Before: pi → [Heap: 500: value 5]
After : pi → [Heap: 500: freed]  (pi still points to old address)
```

---

## 4️⃣ Behavior with Special Cases

| Case                                    | Description                         |
| :-------------------------------------- | :---------------------------------- |
| `free(NULL)`                            | Does nothing (safe to call).        |
| `free(pointer_not_allocated_by_malloc)` | **Undefined behavior** — may crash. |

Example of invalid usage:

```c
int num;
int *pi = &num;
free(pi);   // ❌ Invalid — not heap memory
```

---

## 🧠 Important Rule

> Manage allocation and deallocation at the **same level** —
> if memory is allocated in a function, it should be freed in that same function.

---

## 5️⃣ Avoiding Dangling Pointers

Even after freeing memory, a pointer still contains the old address.
Dereferencing it can crash the program.

To prevent this, **assign `NULL`** to the pointer immediately after freeing:

```c
int *pi = (int *) malloc(sizeof(int));
free(pi);
pi = NULL;
```

✅ Now, `pi` is invalid but safe — dereferencing it will fail immediately instead of accessing garbage.

---

## 🧩 Figure 2-9 — Assigning NULL After Using Free

```
Before: pi → [500: 5]
After : pi = NULL
```

---

## 6️⃣ Double Free Error

A **double free** happens when you attempt to release the same memory twice.
This usually leads to **runtime errors** or heap corruption.

Example 1 — simple case:

```c
int *pi = (int *) malloc(sizeof(int));
free(pi);
free(pi);  // ❌ Runtime error — double free
```

Example 2 — two pointers referencing the same memory:

```c
int *p1 = (int *) malloc(sizeof(int));
int *p2 = p1;

free(p1);
free(p2);  // ❌ Same memory freed twice
```

---

## 🧩 Figure 2-10 — Double Free Example

```
Before:
  p1 → [500: 5]
  p2 → [500: 5]

After:
  Memory freed, but p2 still points to 500 → double free crash
```

💡 **When two pointers refer to the same memory**, this is known as **aliasing**.
Be very careful when freeing such memory.

---

## 7️⃣ The Heap and System Memory

The heap’s space is managed by the operating system.
When you free memory using `free()`, the OS **does not always immediately reclaim it**.
Instead, the freed block becomes available for reuse **within the same program**.

So while your program’s memory usage may appear unchanged from the OS’s perspective,
the heap manager internally marks it as reusable.

---

## 8️⃣ Freeing Memory Upon Program Termination

When a program terminates normally, the **operating system** reclaims all its memory —
including any blocks allocated dynamically.

That means forgetting to `free()` won’t cause leaks after program termination,
but it’s still considered **bad practice** in production code.

| Reason to Free Memory Before Exit | Explanation                                     |
| :-------------------------------- | :---------------------------------------------- |
| Good habit / code quality         | Helps identify real leaks using analysis tools. |
| Long-running applications         | Prevents gradual memory exhaustion.             |
| Reuse within program              | Keeps heap clean and reduces fragmentation.     |

However, in small or short-lived programs, freeing every structure before exit may:

* Be more trouble than it’s worth.
* Add complexity and risk of new bugs.
* Increase runtime slightly.

---

## ✅ Summary

| Concept                | Description                                         |
| :--------------------- | :-------------------------------------------------- |
| **Purpose**            | Releases dynamically allocated memory back to heap. |
| **Pointer after free** | Becomes invalid (dangling).                         |
| **Freeing NULL**       | Safe, does nothing.                                 |
| **Double Free**        | Dangerous — can corrupt memory.                     |
| **Best Practice**      | Always set pointer to `NULL` after freeing.         |
| **Responsibility**     | Allocate and free at the same program level.        |

---

## 🧭 Best Practices

* `free()` only what was allocated with `malloc`, `calloc`, or `realloc`.
* Always check for `NULL` before freeing.
* Immediately set pointer to `NULL` after freeing.
* Avoid freeing memory twice.
* Keep allocation/deallocation logic consistent (same scope or function).

```
