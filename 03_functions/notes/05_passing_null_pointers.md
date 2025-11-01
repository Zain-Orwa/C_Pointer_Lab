````markdown
# 05 — Passing Null Pointers

---

## 1️⃣ What Is a Null Pointer?

A **null pointer** is a pointer that does **not** point to any valid memory location.

```c
int *p = NULL;   // p points to nowhere
````

🔍 Null is not “0 stored in memory.”
It is a special **“no valid address”** marker used for safety.

---

## 2️⃣ Why Null Exists (Real Reasons)

| Purpose            | Explanation                                      |
| ------------------ | ------------------------------------------------ |
| Error signaling    | `malloc()` returns NULL when allocation fails    |
| Defensive coding   | Allows a function to reject invalid input safely |
| Optional arguments | “No buffer passed” → pointer = NULL              |
| Crash prevention   | Better to detect NULL than dereference garbage   |

---

## 3️⃣ Why You MUST Check for NULL

Dereferencing NULL = **undefined behavior**, usually a segmentation fault.

```c
int *p = NULL;
printf("%d\n", *p);   // ❌ CRASH
```

✅ Correct pattern:

```c
if (p != NULL) {
    printf("%d\n", *p);
}
```

NULL = “there is no object to access”

---

## 4️⃣ Passing a Pointer That Might Be NULL

### ❌ Bad function (crashes if NULL passed)

```c
void fillArray(int *arr, int size, int value) {
    for (int i = 0; i < size; i++)
        arr[i] = value;   // ❌ dereference even if arr == NULL
}
```

### ✅ Safe function (checks first)

```c
void fillArray(int *arr, int size, int value) {
    if (arr != NULL) {
        for (int i = 0; i < size; i++)
            arr[i] = value;
    }
}
```

---

## 5️⃣ Full Working Example (Safe)

```c
#include <stdio.h>
#include <stdlib.h>

void fillArray(int *arr, int size, int value) {
    if (arr != NULL) {
        for (int i = 0; i < size; i++)
            arr[i] = value;
    }
}

int main() {
    int *vector = malloc(5 * sizeof(int));
    fillArray(vector, 5, 99);

    for (int i = 0; i < 5; i++)
        printf("%d\n", vector[i]);

    free(vector);
    return 0;
}
```

✅ Works even if malloc returns NULL (because `fillArray` checks)

---

## 6️⃣ Why NULL Matters With `malloc()`

```c
int *p = malloc(5000000000);   // maybe fails
if (p == NULL) {
    printf("Allocation failed!\n");
    return 1;
}
```

❗ Many beginners forget this check → pointer used uninitialized → crash.

---

## 7️⃣ Memory Diagram

```
Before malloc():
+----------+
| vector   | → NULL
+----------+

After malloc():
+----------+         HEAP
| vector   | ────→  [ ][ ][ ][ ][ ]
+----------+

After fillArray():
+----------+         HEAP
| vector   | ────→  [99][99][99][99][99]
+----------+
```

---

## 8️⃣ When Passing NULL Is Valid

```text
| Situation                           | Allowed? | Why                                      |
|-------------------------------------|----------|------------------------------------------|
| Passing NULL to a function that checks it | ✅ Yes  | Defensive code prevents crash            |
| Passing NULL to free()              | ✅ Yes  | free(NULL) is defined and safe           |
| Dereferencing NULL                  | ❌ No   | Causes segmentation fault / UB          |
| Returning NULL on error             | ✅ Yes  | Standard C pattern (signals failure)     |
```

---

## 9️⃣ Common Beginner Mistakes

| Mistake                            | Why it breaks                                |
| ---------------------------------- | -------------------------------------------- |
| Using pointer before checking NULL | Can instantly crash                          |
| Assuming malloc always succeeds    | Wrong — it may fail                          |
| Dereferencing NULL “to test it”    | No, that’s a crash                           |
| Treating NULL as valid buffer      | It is not “empty memory”                     |
| Forgetting NULL != uninitialized   | Uninitialized pointers hold random addresses |

---

## 🔟 Quick Quiz

1. Why is NULL useful in C?
2. What happens if you dereference NULL?
3. Is `free(NULL)` valid?
4. Why must you check the return value of `malloc()`?
5. If a function receives NULL, what should it do?

✅ Answers at bottom.

---

## ✅ Quiz Answers

1. It safely represents “no valid pointer.”
2. Undefined behavior (usually crash).
3. Yes — `free(NULL)` does nothing and is safe.
4. Because malloc can fail and return NULL.
5. Check for NULL and **do nothing or return error**, not crash.

---

## ✅ Summary

* NULL = pointer with **no valid target**
* Always check pointer arguments before use
* `malloc()` can return NULL — never assume success
* `free(NULL)` is always safe
* NULL is a **signal**, not a valid memory location

---

```

