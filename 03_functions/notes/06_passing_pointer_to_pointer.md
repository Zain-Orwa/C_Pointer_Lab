````markdown
# 06 — Passing a Pointer to a Pointer

---

## 1️⃣ Why Do We Need a Pointer to a Pointer?

In C, **all function parameters are passed by value** — even pointers.

So if we pass a normal pointer like this:

```c
void allocateArray(int *arr, int size, int value);
````

The function receives **a copy of the pointer**, not the original one from `main()`.
That means if the function modifies the pointer (e.g. points it to malloc’d memory),
the caller **will NOT see the change**.

✅ Solution → **Pass the address of the pointer itself**:

```c
void allocateArray(int **arr, int size, int value);
```

Now the function receives a pointer **to** the pointer, so it can modify the original one in `main()`.

---

## 2️⃣ Correct Version — Using Pointer to Pointer ✅

```c
#include <stdio.h>
#include <stdlib.h>

void allocateArray(int **arr, int size, int value) {
    *arr = (int*)malloc(size * sizeof(int));
    if (*arr != NULL) {
        for (int i = 0; i < size; i++) {
            (*arr)[i] = value;
        }
    }
}

int main() {
    int *vector = NULL;

    allocateArray(&vector, 5, 45);

    for (int i = 0; i < 5; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");

    free(vector);
    return 0;
}
```

✅ Output:

```
45 45 45 45 45
```

✅ `vector` now points to valid heap memory
✅ No memory leak (because we `free()` it)

---

## 3️⃣ Memory Diagram — Correct Version

```
Before return (inside allocateArray)

+------------------------------- stack -------------------------------+
| allocateArray frame                                                |
|   arr (int**) ----> 0x700                                          |
|   *arr (int*) ----> 0x600                                          |
+--------------------------------------------------------------------+
| main frame                                                         |
|   vector  = NULL                                                   |
+------------------------------- heap --------------------------------+
| 0x600: [45][45][45][45][45]                                        |
+--------------------------------------------------------------------+


After return (back in main)

+------------------------------- stack -------------------------------+
| main frame                                                         |
|   vector ----> 0x600 ✅ (updated!)                                 |
+------------------------------- heap --------------------------------+
| 0x600: [45][45][45][45][45]  (still valid until free())            |
+--------------------------------------------------------------------+
```

---

## 4️⃣ Incorrect Version — Passing a Normal Pointer ❌

```c
#include <stdio.h>
#include <stdlib.h>

void allocateArray(int *arr, int size, int value) {
    arr = (int*)malloc(size * sizeof(int));
    if (arr != NULL) {
        for (int i = 0; i < size; i++) {
            arr[i] = value;
        }
    }
}

int main() {
    int *vector = NULL;

    allocateArray(vector, 5, 45);
    printf("%p\n", vector);   // prints 0x0 (still NULL!)
}
```

❌ `vector` is still NULL
❌ Caller does **not** receive the new address
❌ Heap memory is allocated but LOST → **memory leak**

---

### Memory Diagram — Wrong Version (Leak)

```
Before return (inside allocateArray)

+---------------------------- stack ----------------------------+
| allocateArray frame                                           |
|   arr (copy) ----> 0x600                                      |
+---------------------------------------------------------------+
| main frame                                                    |
|   vector = NULL                                               |
+---------------------------- heap -----------------------------+
| 0x600: [45][45][45][45][45]  (allocated but unreachable) ❌    |
+---------------------------------------------------------------+

After return

+---------------------------- stack ----------------------------+
| main frame                                                    |
|   vector = NULL ❌ (never updated)                            |
+---------------------------------------------------------------+
| 0x600: [45][45][45][45][45]  (lost forever → leak) 💀         |
+---------------------------------------------------------------+
```

---

## 5️⃣ Comparison Table

```
| Function Parameter Type | Works? | Caller Pointer Updated?  | Memory Leak Risk |
|-------------------------|--------|--------------------------|------------------|
| int *p                  | ❌ No  | ❌ No                    | ✅ Yes (likely)  |
| int **p                 | ✅ Yes | ✅ Yes                   | ❌ No (if freed) |
```

---

## 6️⃣ Key Takeaways

* ✔️ To modify a pointer inside a function, you must pass `&pointer` and use `int **`
* ✔️ `malloc()` inside a function requires **pointer to pointer** if caller will use result
* ❌ Passing a simple pointer only modifies a copy → caller stays unchanged
* ⚠️ Wrong version causes **memory leak** because pointer to heap is lost
* ✅ Always `free()` the memory back in the caller

---

## 7️⃣ Mini-Quiz

1. Why does `int *p` fail when allocating memory inside a function?
2. What does `*arr` mean inside `allocateArray(int **arr, ...)`?
3. Where does the allocated memory live — stack or heap?
4. Who must call `free()` — the function, or the caller?
5. What happens if we forget to use `&vector` when calling?


```

