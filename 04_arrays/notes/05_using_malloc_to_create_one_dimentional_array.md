# 🧩 Chapter 4 — Lesson 5: Using `malloc()` to Create a One-Dimensional Array

---

## 1️⃣ Overview

Until now, arrays were **statically allocated** (created at compile time).
Using `malloc()` lets us allocate arrays **dynamically** — that is, at **runtime**, from the **heap** instead of the stack.

🧠 **Core idea:**
When you allocate memory with `malloc()`, it returns a **pointer** to a contiguous region of memory.
That pointer can then be treated just like an array name.

---

## 2️⃣ Example: Allocating and Initializing an Array on the Heap

```c
int *pv = (int *) malloc(5 * sizeof(int));

for (int i = 0; i < 5; i++) {
    pv[i] = i + 1;
}
```

✅ This dynamically allocates space for **5 integers** (5 × 4 bytes = 20 bytes) and initializes them with 1 through 5.

---

## 3️⃣ Alternative Notation (Using Pointer Arithmetic)

```c
for (int i = 0; i < 5; i++) {
    *(pv + i) = i + 1;
}
```

Both forms (`pv[i]` and `*(pv+i)`) store the same values, but the first is clearer and more common in real code.

---

## 4️⃣ Memory Layout

📘 **Figure 4-5 — Array Allocated from the Heap**

```
   Program Stack                     Heap
   --------------            ----------------------
   pv @ 100   ---> 500 ----> [ 1 ][ 2 ][ 3 ][ 4 ][ 5 ]
                              ^ base address (500)
```

🧩 Explanation:

* `pv` is a pointer variable stored on the stack (at address 100).
* It holds the value `500`, the starting address of the allocated block on the heap.
* The heap stores five consecutive `int` values (4 bytes each).

---

## 5️⃣ Pointer Precedence Clarification

⚠️ Common mistake when writing pointer expressions:

```c
*(pv + i)   // ✅ correct — add i first, then dereference
*pv + i     // ❌ incorrect — dereferences first, then adds i to the value
```

🧠 The dereference operator (`*`) has **higher precedence** than the addition operator (`+`).
That’s why parentheses are required to ensure the addition is done first.

---

### Example Demonstration

Let’s say `pv = 500` and `i = 2`.

| Expression  | Step 1                         | Step 2            | Result         |
| :---------- | :----------------------------- | :---------------- | :------------- |
| `*(pv + i)` | `pv + i → 500 + (2 × 4) = 508` | dereference 508   | value = 3      |
| `*pv + i`   | dereference 500 → 1            | add 2 → 1 + 2 = 3 | wrong semantic |

🧠 Although the numeric result might look same here by coincidence, `*pv + i` does not fetch the i-th element — it just adds `i` to the first element’s value.

---

## 6️⃣ Deallocation

Every `malloc()` allocation must be paired with a `free()` to avoid memory leaks.

```c
free(pv);
pv = NULL;   // optional but good habit
```

📘 **Rule of thumb:**

* Always `free()` heap memory once you’re done using it.
* Never `free()` stack variables or the same pointer twice.

---

## 7️⃣ Comparison — Stack vs Heap Allocation

| Aspect                      | Stack Array                 | Heap Array (using `malloc`)           |
| :-------------------------- | :-------------------------- | :------------------------------------ |
| Lifetime                    | Automatic (end of function) | Manual (`free()` required)            |
| Scope                       | Limited to function         | Accessible globally via pointer       |
| Location                    | Stack segment               | Heap segment                          |
| Size known at compile time? | ✅ Yes                       | ❌ No — decided at runtime             |
| Syntax                      | `int arr[5];`               | `int *arr = malloc(n * sizeof(int));` |

---

## 8️⃣ Memory Visualization Step-by-Step

### Before `malloc()`

```
Stack:
  pv   →  (undefined)
Heap:
  [ empty ]
```

### After Allocation

```
Stack:
  pv   →  500
Heap:
  500  [ _ ][ _ ][ _ ][ _ ][ _ ]
```

### After Initialization

```
Stack:
  pv   →  500
Heap:
  500  [1][2][3][4][5]
```

### After `free(pv)`

```
Stack:
  pv   →  NULL
Heap:
  (deallocated)
```

---

## 9️⃣ Why Use `malloc()` for Arrays?

| Reason                | Benefit                                          |
| :-------------------- | :----------------------------------------------- |
| Runtime flexibility   | Size can be determined dynamically               |
| Large data sets       | Heap can store more data than stack              |
| Inter-function access | Pointer can be passed between functions          |
| Reusability           | Enables dynamic array management (e.g., realloc) |

---

## 🔟 Summary

| Concept              | Description                                    |
| :------------------- | :--------------------------------------------- |
| `malloc()`           | Allocates raw memory from the heap             |
| `pv[i] = *(pv+i)`    | Array notation and pointer notation equivalent |
| `*(pv+i)` vs `*pv+i` | Parentheses define operation order             |
| Heap memory          | Must be freed manually                         |
| Pointer on stack     | Holds address of heap memory region            |

---

### 🧠 Key Takeaway

> Using `malloc()` lets you create arrays whose size is known only at runtime.
> Once allocated, they behave exactly like normal arrays — just remember to `free()` them when you’re done.

---


