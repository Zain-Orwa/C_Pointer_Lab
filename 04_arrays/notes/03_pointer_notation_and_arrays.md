# 🧩 Chapter 4 — Lesson 3: Pointer Notation and Arrays

---

## 1️⃣ Overview

Arrays and pointers in C are **tightly connected**, and you can often use them interchangeably.
However, they are **not identical**, and understanding their subtle differences is crucial for safe and efficient code.

🧠 **Key idea:**
Array names can act like constant pointers to their first elements, but they **cannot be reassigned** or **incremented**.

---

## 2️⃣ Array-to-Pointer Relationship

When an array name appears by itself in an expression, it **decays into a pointer** to its first element.

```c
int vector[5] = {1, 2, 3, 4, 5};
int *pv = vector;  // equivalent to int *pv = &vector[0];
```

✅ `pv` now points to `vector[0]`, not the array itself.
✅ You can move `pv` using pointer arithmetic (unlike `vector`).

---

📘 **Memory Layout**

```
Address   Value   Expression
-------   -----   -----------
100       1       vector[0]
104       2       vector[1]
108       3       vector[2]
112       4       vector[3]
116       5       vector[4]
```

🧩 **Relationships**

| Expression   | Meaning                   | Result |
| :----------- | :------------------------ | :----- |
| `vector`     | address of first element  | `100`  |
| `&vector[0]` | address of first element  | `100`  |
| `pv`         | address stored in pointer | `100`  |

---

## 3️⃣ Address of the Whole Array

The expression `&vector` has a different meaning from `vector` or `&vector[0]`.

| Expression | Type         | Points To                                       |
| :--------- | :----------- | :---------------------------------------------- |
| `vector`   | `int*`       | `vector[0]`                                     |
| `&vector`  | `int (*)[5]` | The entire array (a pointer to array of 5 ints) |

🧠 This distinction becomes important when passing **multi-dimensional arrays** to functions.

---

## 4️⃣ Equivalent Pointer Access

The following notations are equivalent for accessing array elements:

```c
vector[i]    ≡    *(vector + i)
pv[i]        ≡    *(pv + i)
```

These forms both use **pointer arithmetic** internally.

---

📘 **Memory Layout — Pointer Arithmetic**

```
Base address = 100
sizeof(int)  = 4

*(vector + 0) → 100 → 1
*(vector + 1) → 104 → 2
*(vector + 2) → 108 → 3
*(vector + 3) → 112 → 4
*(vector + 4) → 116 → 5
```

🧠 Every time you add `1` to a pointer to `int`,
you move **4 bytes forward** (the size of an integer).

---

## 5️⃣ Table — Array vs Pointer Notation

| Value | Equivalent Expression                     | Meaning                                     |
| :---- | :---------------------------------------- | :------------------------------------------ |
| 92    | `&vector[-2]` / `vector - 2` / `pv - 2`   | Access 2 elements before start (⚠️ invalid) |
| 100   | `vector` / `&vector[0]` / `pv`            | Base address                                |
| 104   | `vector + 1` / `&vector[1]` / `pv + 1`    | Next element                                |
| 140   | `vector + 10` / `&vector[10]` / `pv + 10` | 10 elements ahead (⚠️ invalid)              |

📘 **Figure 4-4 — Array/Pointer Notation**

```
Address   vector[i]   Equivalent Pointer
----------------------------------------
  92     vector[-2]   pv - 2     (⚠️ invalid)
 100     vector[0]    pv         base
 104     vector[1]    pv + 1
 108     vector[2]    pv + 2
 112     vector[3]    pv + 3
 116     vector[4]    pv + 4
 140     vector[10]   pv + 10    (⚠️ invalid)
```

🧠 Pointer arithmetic simply moves across memory based on element size.
Adding `1` moves 4 bytes (for `int`), adding `2` moves 8 bytes, etc.

---

## 6️⃣ Understanding “Shift and Dereference”

You can think of `vector[i]` as **shift + dereference**:

1. Shift `i` positions from the start (`vector + i`)
2. Dereference the result (`*`)

```c
vector[2]   →   *(vector + 2)
&vector[2]  →   (vector + 2)
```

🧩 **Analogy:**
It’s like saying: “move two boxes to the right, then open that box.”

---

## 7️⃣ Using Pointers for Scalar Operations

Let’s use a pointer to modify all elements in an array directly.

```c
int vector[5] = {1, 2, 3, 4, 5};
int *pv = vector;
int value = 3;

for (int i = 0; i < 5; i++) {
    *pv++ *= value;
}
```

🧠 `*pv++` means:

* Dereference `pv` (access element)
* Then increment pointer to the next element

---

📘 **Memory Evolution**

| Step | pv points to | Operation | Result |
| :--- | :----------- | :-------- | :----- |
| 1    | vector[0]    | `1 * 3`   | `3`    |
| 2    | vector[1]    | `2 * 3`   | `6`    |
| 3    | vector[2]    | `3 * 3`   | `9`    |
| 4    | vector[3]    | `4 * 3`   | `12`   |
| 5    | vector[4]    | `5 * 3`   | `15`   |

Final array contents:

```
{3, 6, 9, 12, 15}
```

---

📘 **Memory Layout — Pointer Iteration**

```
Iteration 0: pv → 100
Iteration 1: pv → 104
Iteration 2: pv → 108
Iteration 3: pv → 112
Iteration 4: pv → 116
```

Each step shifts 4 bytes forward.

---

## 8️⃣ Key Rules of Pointer and Array Notation

| Rule                          | Explanation                        |
| :---------------------------- | :--------------------------------- |
| Array name = pointer constant | Cannot be reassigned               |
| `vector[i] == *(vector + i)`  | True for all valid indices         |
| `&vector[i] == vector + i`    | Address equivalence                |
| Pointer arithmetic scales     | Adds `(n × sizeof(type))` bytes    |
| `*pv++`                       | Dereference then increment pointer |
| `++*pv`                       | Increment value, not pointer       |

🧠 Parentheses matter — `*pv++` ≠ `(*pv)++`.

---

## 9️⃣ Common Pitfalls

| Mistake                                      | Consequence                                         |
| :------------------------------------------- | :-------------------------------------------------- |
| Accessing `vector[-1]` or `vector[10]`       | Undefined behavior                                  |
| Using `sizeof` on a pointer instead of array | Returns pointer size, not array size                |
| Forgetting to initialize pointer             | Segmentation fault                                  |
| Misusing `&vector`                           | Produces pointer to entire array, not first element |

---

## 🔟 Summary

| Concept                    | Description                            |
| :------------------------- | :------------------------------------- |
| `vector` vs `pv`           | Array name vs pointer variable         |
| Pointer arithmetic         | Moves across memory using element size |
| `vector[i]`                | Equivalent to `*(vector + i)`          |
| Dereferencing              | Accesses the data at that address      |
| Pointer increment (`pv++`) | Advances to next element               |
| Pointer safety             | Always stay within bounds              |

---

### 🧠 Key Takeaway

> Arrays and pointers are two faces of the same memory structure.
> Arrays provide convenient syntax; pointers provide flexibility and power.
> Mastering both lets you manipulate data at the memory level with precision and control.

---

