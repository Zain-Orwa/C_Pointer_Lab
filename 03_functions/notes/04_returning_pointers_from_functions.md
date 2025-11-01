```markdown
# 04 — Returning Pointers from Functions

When you return a pointer, you’re not returning data — you’re returning an **address**.  
If that address refers to memory that won’t exist after the function returns, you’ve built a bug. Full stop.

---

## 📌 Quick Rules

1. ✅ You may return **heap** memory (`malloc`, `calloc`).
2. ❌ You must **not** return the address of **local (stack)** variables/arrays.
3. ✅ You can return **static** data, but it’s shared across calls (last write wins).
4. ✅ If the callee allocates, the **caller frees** — document ownership.
5. ✅ Prefer **caller-provided buffers** when size is known or performance matters.

---

## 🧠 Why Returning Pointers Is Tricky

- A pointer is just an address; the **pointee’s lifetime** matters.
- **Stack** storage dies when the function returns.
- **Heap** storage lives until `free()`.
- **Static** storage lives for the entire program, but it’s shared.

```

| Storage Class | Lifetime               | Safe to Return? | Notes                                 |
| ------------- | ---------------------- | --------------- | ------------------------------------- |
| Stack         | Until function returns | ❌ No            | Local vars/arrays die with the frame  |
| Heap          | Until caller frees     | ✅ Yes           | Caller owns lifetime                  |
| Static        | Entire program         | ✅ Yes           | Shared buffer; last writer overwrites |
| Global        | Entire program         | ✅ Yes           | Avoid global state if you can         |

```

---

## 🧱 Memory Diagrams (Mental Model)

**Stack vs Heap on a call returning an array:**

```
Before return (inside callee):
┌───────────────────────────────┐
│  callee() stack frame         │
│  ───────────────────────────  │
│  int arr[5];        @ 0x700   │
│  arr: [45][45][45][45][45]    │
│                               │
│  return arr;   →  pointer=0x700
└───────────────────────────────┘

After return (back in caller):
┌───────────────────────────────┐
│  caller() stack frame         │
│  ───────────────────────────  │
│  int *vec = 0x700   ← still points here ❌
└───────────────────────────────┘

But callee() frame is gone now!

Memory at 0x700 is no longer valid.
That region of the stack is free to be reused (garbage).

#Correct version using malloc:
```
┌───────────────────────────────┐
│  callee() stack frame         │
│  int *arr = malloc(...) @0x500│
│  arr: [45][45][45][45][45]    │
│  return arr;   → pointer=0x500│
└───────────────────────────────┘

After return:
┌───────────────────────────────┐
│  caller() frame               │
│  int *vec = 0x500 ✅ valid    │
│  vec still points to HEAP     │
└───────────────────────────────┘

HEAP memory persists until free(vec)

````

---

## ✅ Concept A — Return Heap Memory (Correct & Common)

Allocate on the heap inside the function and return the pointer. The caller frees it.

```c
// 04A_heap_return.c
#include <stdio.h>
#include <stdlib.h>

int* allocateArray_heap(int size, int value) {
    int *arr = malloc((size_t)size * sizeof *arr);
    if (!arr) return NULL;
    for (int i = 0; i < size; ++i) arr[i] = value;
    return arr; // ✅ survives after return
}

int main(void) {
    int n = 5;
    int *vector = allocateArray_heap(n, 45);
    if (!vector) {
        perror("malloc failed");
        return 1;
    }
    for (int i = 0; i < n; ++i)
        printf("vector[%d] = %d\n", i, vector[i]);
    free(vector); // ✅ caller cleans up
    return 0;
}
````

**Why this works (step-by-step):**

1. `malloc` grabs space from the **heap**.
2. Function returns the **address** of that heap block.
3. Callee’s stack frame dies, but the **heap block persists**.
4. Caller uses it, then **free()**s it exactly once.

---

## ❌ Concept B — Returning Local (Stack) Memory (Wrong)

Classic foot-gun: returning the address of a local array/variable.

```c
// 04_bad_returning_stack.c
#include <stdio.h>

int* allocateArray_bad(int size, int value) {
    int arr[5];                  // stack storage
    if (size > 5) size = 5;
    for (int i = 0; i < size; ++i) arr[i] = value;
    return arr;                  // ❌ returns address to dead storage
}

int main(void) {
    int *p = allocateArray_bad(5, 42);
    // Undefined behavior: p points to invalid memory
    for (int i = 0; i < 5; ++i) printf("%d\n", p[i]); // may print garbage or crash
}
```

**Why it fails:**

* The local `arr` lives **only** while `allocateArray_bad` runs.
* After return, that stack region is fair game for reuse.
* Access is undefined. Sometimes “works” until it doesn’t.

---

## ✅ Alternative — Caller Allocates, Callee Fills (Often Best)

Avoids hidden allocation and ownership confusion. Great for performance and clarity.

```c
// 04B_caller_buffer.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool fillArray(int *out, int size, int value) {
    if (!out || size < 0) return false;
    for (int i = 0; i < size; ++i) out[i] = value;
    return true;
}

int main(void) {
    int n = 5;
    int *buf = malloc((size_t)n * sizeof *buf); // caller owns storage
    if (!buf) return 1;

    if (!fillArray(buf, n, 45)) { free(buf); return 1; }

    for (int i = 0; i < n; ++i)
        printf("buf[%d] = %d\n", i, buf[i]);

    free(buf);
    return 0;
}
```

**Why this is solid:**

* Ownership is clear: caller allocates & frees.
* No hidden `malloc` cost inside the callee.
* Enables reuse of fixed buffers in tight loops.

---

## ⚠️ Niche — Return a Static Local (Shared Storage)

Safe lifetime, but the **same buffer** is returned each call. Last write wins.

```c
// 04C_static_shared.c
#include <stdio.h>

int* allocateArray_static(int size, int value) {
    static int arr[5];              // static storage duration
    if (size > 5) size = 5;
    for (int i = 0; i < size; ++i) arr[i] = value;
    return arr;                     // ✅ lifetime OK, ⚠ shared
}

int main(void) {
    int *a = allocateArray_static(5, 11);
    int *b = allocateArray_static(5, 22); // overwrites same array
    for (int i = 0; i < 5; ++i)
        printf("a[%d]=%d  b[%d]=%d\n", i, a[i], i, b[i]);
    // no free(); static storage is global lifetime
    return 0;
}
```

**Use when:**

* You’re returning small, fixed-size status buffers or constant-ish data.
* You accept that subsequent calls overwrite previous results.
* You’re explicit in docs: “This returns a pointer to a shared static buffer.”

---

## 🧮 Decision Table

```
| Situation                                 | Pick This                             | Why                                             |
|-------------------------------------------|---------------------------------------|--------------------------------------------------|
| Need independent result per call          | Callee `malloc` + return pointer      | Fresh heap block; caller controls lifetime       |
| Known size / performance-sensitive path   | Caller allocates; callee fills        | No hidden allocs; simple ownership               |
| Tiny fixed reuse, shared OK               | `static` local return                 | No heap cost; shared buffer trade-off            |
| You considered returning stack memory     | Don’t. Redesign.                      | That’s undefined behavior                        |
```

---

## 🛑 Common Mistakes (and the fix)

* Returning address of a local array/variable → **don’t**. Use heap or caller buffer.
* Forgetting to `free()` heap memory → **document ownership** and audit.
* Double `free()` on same pointer or its alias → **one and only one** `free()`.
* Not checking `malloc` result → check `if (!p)` before use.
* Overflow in `n * sizeof(T)` → validate sizes before allocation.
* Mixing `realloc` result into original pointer directly → use a **temp** pointer first.

---

## 📝 Mini-Quiz

1. Can you return the address of a local array? Why not?
2. Who frees memory returned by a callee that used `malloc`?
3. What’s the pitfall of returning a `static` buffer?
4. Which is clearer in ownership: callee-allocates or caller-allocates?
5. What’s the right fix if you need a fresh array every call?

**Answers:**

1. No; stack memory dies at return → dangling pointer.
2. The **caller**.
3. It’s **shared**; later calls overwrite earlier results.
4. **Caller-allocates** tends to be clearest and fastest.
5. Callee allocates on the **heap** and returns the pointer; caller frees.

---

## 🧩 Exercises

1. Rewrite a buggy “return local array” function into a callee-heap version.
2. Implement `int *dup_ints(const int *src, int n)` that returns a heap copy (or `NULL`); write a `main` that tests and frees it.
3. Swap `malloc+loop` with `calloc` in `allocateArray_heap`, benchmark large `n`.

---

```

