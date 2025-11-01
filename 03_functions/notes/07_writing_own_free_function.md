# 06_writing_own_free_function.md

## 1. Overview

`free()` deallocates a heap block, **but it does not**:

* check whether the pointer is `NULL`, and
* set the pointer to `NULL` after freeing.

That omission is the classic root of **use-after-free** and **double free** bugs.
This lesson shows a small, portable pattern that makes freeing safer:

* a **function** that takes a **pointer to a pointer** (`void **`) and nullls it, and
* a convenience **macro** that preserves type safety at the call site.

---

## 2. Problems with `free()`

### 2.1 Symptoms you eventually hit

* **Dangling pointer:** object is freed; variable still holds the old address.
* **Double free:** the same address is freed twice.
* **Missing `NULL` check:** calling `free()` on an uninitialized pointer is UB if it’s not actually `NULL`.

### 2.2 Why `free()` can’t null your pointer

C passes arguments **by value**. Inside `free(p)`, the callee sees a *copy* of the address, not your variable.
So even if `free` wanted to write `p = NULL;`, it would only null its **local copy**, not yours.

---

## 3. Pointer-to-pointer solution

To modify the caller’s pointer, pass its **address** → type `void **`.
Why `void **`? Because it can receive the address of `T *` (e.g., `int *`, `char *`) after an explicit cast.
(We’ll hide the cast behind a macro to keep call sites clean and type-checked.)

**Mini diagram (conceptual):**

```
caller variable                callee parameter
+-----------+                  +-----------+
|  p: 0x500 | -- address -->   | pp: &p    |
+-----------+                  +-----------+

saferFree(&p) can write through *pp and set caller's p = NULL
```

---

## 4. Implementing `saferFree()` (function)

```c
/* safer_free.h */
#ifndef SAFER_FREE_H
#define SAFER_FREE_H

#include <stdlib.h>  /* free   */
#include <stddef.h>  /* NULL   */

/* Do NOT call this directly; use SAFER_FREE(p) macro below. */
static inline void saferFreeImpl(void **pp)
{
    if (pp != NULL && *pp != NULL) {
        free(*pp);     /* deallocate the heap block         */
        *pp = NULL;    /* kill the dangling pointer hazard  */
    }
}

/* Public macro: preserves type intent at call site. */
#define SAFER_FREE(p) saferFreeImpl((void **)&(p))

#endif /* SAFER_FREE_H */
```

### Notes

* `static inline` keeps the helper in headers without ODR/link issues.
* The macro guarantees we always pass the **address** of the pointer variable and performs the required `(void **)` cast in one place.

---

## 5. Complete demo (compilable)

```c
/* demo_safer_free.c */
#include <stdio.h>
#include <stdlib.h>
#include "safer_free.h"

int main(void)
{
    int *pi = malloc(sizeof *pi);
    if (!pi) {
        perror("malloc");
        return 1;
    }
    *pi = 5;

    printf("Before:  pi = %p, *pi = %d\n", (void*)pi, *pi);

    /* First free: releases memory and nulls the pointer. */
    SAFER_FREE(pi);
    printf("After1:  pi = %p\n", (void*)pi);

    /* Second free: safe no-op because pi == NULL. */
    SAFER_FREE(pi);
    printf("After2:  pi = %p (still NULL, no crash)\n", (void*)pi);

    return 0;
}
```

**Expected output (addresses will differ):**

```
Before:  pi = 0x100804000, *pi = 5
After1:  pi = (nil)
After2:  pi = (nil) (still NULL, no crash)
```

---

## 6. Macro vs. direct function call

You *could* expose a function `void saferFree(void **pp)`.
However, each call would require an explicit cast at call site:

```c
/* Clunky and error-prone: */
saferFree((void **)&pi);
```

The `SAFER_FREE(pi)` macro:

* hides the cast,
* ensures we pass an **lvalue pointer variable** (not a temporary),
* remains readable in code reviews.

---

## 7. What this guards (and what it can’t)

### 7.1 Guarded

| Situation                    | With plain `free(p)` | With `SAFER_FREE(p)` | Why                                           |
| :--------------------------- | :------------------- | :------------------- | :-------------------------------------------- |
| Free once, forget to null    | ⚠️ Dangling pointer  | ✅ Becomes `NULL`     | Assigns `NULL` through the pointer-to-pointer |
| Double free on same variable | 💥 Likely crash/UB   | ✅ Safe no-op         | Second call sees `NULL` and returns           |
| Conditional free paths       | 😬 Easy to miss null | ✅ Uniform behavior   | Centralizes the “free+null” pattern           |

### 7.2 Not guarded

| Situation                                 | Protected? | Notes                                                                                 |
| :---------------------------------------- | :--------- | :------------------------------------------------------------------------------------ |
| Two **aliases** to the same heap block    | ❌          | Nulling `p` doesn’t null `q` (alias). Fix by nulling *all* owning aliases or redesign |
| Freeing a **stack** or **static** address | ❌          | Still UB. Only pass pointers you obtained from `malloc/calloc/realloc`                |
| Use-after-free via **copied raw address** | ❌          | If a raw copy of the address escaped, nulling the owner won’t save the copy           |

---

## 8. Memory timeline (ASCII)

```
(1) Allocate
 p ──► 0x600 ── heap(int=5)

(2) SAFER_FREE(p)
 free(0x600)  ;  p ← NULL

(3) Second SAFER_FREE(p)
 if (p == NULL) return;  // no-op
```

---

## 9. Common mistakes

1. **Passing the pointer value instead of its address**

   ```c
   /* WRONG: */ saferFreeImpl((void**)pi);     // pi is not an address of a pointer
   /* RIGHT: */ saferFreeImpl((void**)&pi);    // address-of the pointer variable
   ```

2. **Freeing non-heap memory**

   ```c
   int x; SAFER_FREE(&x);   // ❌ UB. Never free stack/static.
   ```

3. **Assuming aliases are nulled**

   ```c
   int *a = malloc(4);
   int *b = a;
   SAFER_FREE(a);
   /* b is still dangling → do not use; either null b or avoid aliasing ownership. */
   ```

---

## 10. Best practices (concise)

* Prefer **single owner** for each heap object. If you must alias, define clear ownership rules.
* After `realloc`, assign to a **temporary** first to avoid leaks on failure:

  ```c
  int *tmp = realloc(p, newsz);
  if (tmp) p = tmp;
  ```
* Use `SAFER_FREE(p)` in all cleanup paths (including `goto`-based error unwinding).
* Keep pointers **initialized** (`T *p = NULL;`) until first allocation.

---

## 11. Unit-test style snippets

```c
/* Freeing NULL is a no-op. */
int *p = NULL;
SAFER_FREE(p);  /* ok */

/* Double-free avoidance. */
int *q = malloc(16);
SAFER_FREE(q);
SAFER_FREE(q);  /* ok, q already NULL */

/* Array case. */
int *v = malloc(10 * sizeof *v);
for (int i = 0; i < 10; ++i) v[i] = i;
SAFER_FREE(v);  /* frees whole block and nulls v */
```

---

## 12. FAQ

**Q: Why not just `p = NULL; free(p);` everywhere?**
**A:** That order is wrong—`free(NULL)` is a no-op. You must `free(p)` **before** nulling, and you must do it **everywhere**. Encapsulating in `SAFER_FREE` removes human error.

**Q: Why `void **` and not `T **`?**
**A:** The helper is generic. The macro keeps call sites type-checked while funneling to a single `void **` implementation.

**Q: Is this overkill?**
**A:** In small programs, maybe. In larger codebases, “free+null” as a single idiom prevents a class of bugs with negligible cost.

---

## 13. Summary

* `free()` can’t null your variable; arguments are by value.
* Pass the **address of your pointer** (`void **`) to let the callee set it to `NULL`.
* Use the `SAFER_FREE(p)` macro to keep call sites clean and consistent.
* This pattern eliminates many dangling-pointer and double-free mistakes without changing your allocator.

---


```
