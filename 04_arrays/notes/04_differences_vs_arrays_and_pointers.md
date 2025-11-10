# 🧩 Chapter 4 — Lesson 4: Differences Between Arrays and Pointers

---

## 1️⃣ Overview

Arrays and pointers in C **behave similarly**, but they are **not identical**.
They can often be used interchangeably in expressions, yet they differ in **memory representation**, **`sizeof` behavior**, and **mutability**.

🧠 **Short summary:**

* Arrays are **fixed blocks of memory**.
* Pointers are **variables that store memory addresses**.

---

## 2️⃣ Example Setup

```c
int vector[5] = {1, 2, 3, 4, 5};
int *pv = vector;
```

Now:

* `vector` → base address of the array
* `pv` → pointer variable pointing to the same first element

---

## 3️⃣ Expression Differences

Although `vector[i]` and `*(vector + i)` return the same **value**, they produce **different machine instructions**.

| Expression      | Description                                                       |
| :-------------- | :---------------------------------------------------------------- |
| `vector[i]`     | Access via array indexing syntax (compiler translates internally) |
| `*(vector + i)` | Access via pointer arithmetic and dereference                     |

🧠 Both yield identical results at runtime — only the compiler’s intermediate code differs.

---

📘 **Memory Layout**

```
Address   Value   Expression
-------   -----   -----------
100       1       vector[0] / *(vector + 0)
104       2       vector[1] / *(vector + 1)
108       3       vector[2] / *(vector + 2)
112       4       vector[3] / *(vector + 3)
116       5       vector[4] / *(vector + 4)
```

---

## 4️⃣ `sizeof` Behavior Difference

The `sizeof` operator behaves **differently** for arrays and pointers:

| Expression       | Result                 | Explanation                |
| :--------------- | :--------------------- | :------------------------- |
| `sizeof(vector)` | `20`                   | Array occupies 5 × 4 bytes |
| `sizeof(pv)`     | `4` (or `8` on 64-bit) | Pointer size only          |

🧠 **Rule:**

* When used with an array, `sizeof` returns the total **allocated memory**.
* When used with a pointer, it returns only the **pointer’s own size**.

---

## 5️⃣ Modifiability — `lvalue` vs Non-`lvalue`

A **pointer** is an *lvalue* — meaning it can appear on the **left-hand side** of an assignment.
An **array name** is **not** an lvalue — its base address is fixed and cannot be changed.

```c
pv = pv + 1;      // ✅ Valid
vector = vector + 1;  // ❌ Syntax error — array name not modifiable
```

🧠 **Reason:**
An array name represents a **constant address** in memory,
while a pointer variable holds a **modifiable copy of an address**.

---

📘 **Analogy**

| Concept | Analogy                                   |
| :------ | :---------------------------------------- |
| Array   | A building’s fixed foundation             |
| Pointer | A visitor holding a map — can go anywhere |

---

## 6️⃣ Expression Validity

Even though `vector = vector + 1` is illegal,
the expression `vector + 1` **is perfectly fine** in calculations — it simply returns an address offset.

```c
printf("%p\n", vector);      // prints base address (e.g., 0x100)
printf("%p\n", vector + 1);  // prints base + 4 bytes (0x104)
```

🧠 `vector + 1` means “address of next integer” — but you can’t assign that result to `vector`.

---

📘 **Memory Layout — Pointer Movement**

```
   vector (base) ----> [1][2][3][4][5]
                         ↑
                         |
                        pv

After pv = pv + 1:

   vector (base) ----> [1][2][3][4][5]
                              ↑
                              |
                             pv (now points to vector[1])
```

---

## 7️⃣ Practical Summary

| Concept                 | Array (`vector`)         | Pointer (`pv`)              |
| :---------------------- | :----------------------- | :-------------------------- |
| Memory storage          | Fixed block (contiguous) | Variable holding an address |
| Can reassign            | ❌ No                     | ✅ Yes                       |
| `sizeof` result         | Total size of array      | Size of pointer             |
| Lvalue                  | ❌ No                     | ✅ Yes                       |
| Access syntax           | `vector[i]`              | `pv[i]` or `*(pv+i)`        |
| Expression `vector+1`   | Valid (address math)     | Valid (pointer shift)       |
| Assignment `vector=...` | Invalid                  | Valid                       |

---

## 8️⃣ Visual Comparison

📘 **Array vs Pointer Diagram**

```
Array (Fixed in Memory)
-----------------------
vector → [1][2][3][4][5]
^
| (constant address)

Pointer (Movable)
-----------------
pv → [1][2][3][4][5]
     ^
     |
    can move to next element with pv++
```

🧠 The array is **anchored**; the pointer is **mobile**.

---

## 9️⃣ Code Recap

```c
int vector[5] = {1, 2, 3, 4, 5};
int *pv = vector;

printf("sizeof(vector): %zu\n", sizeof(vector)); // 20
printf("sizeof(pv): %zu\n", sizeof(pv));         // 8 (on 64-bit)

pv = pv + 1;     // ✅ moves pointer
// vector = vector + 1;  ❌ invalid
```

Output (on 64-bit system):

```
sizeof(vector): 20
sizeof(pv): 8
```

---

## 🔟 Summary

| Key Difference         | Explanation                                                           |
| :--------------------- | :-------------------------------------------------------------------- |
| **Address mutability** | Pointer variable can move; array base cannot.                         |
| **`sizeof`**           | Returns total array size vs pointer size.                             |
| **Storage**            | Array lives in contiguous static memory; pointer lives independently. |
| **Lvalue**             | Pointer is lvalue; array name is not.                                 |

---

### 🧠 Key Takeaway

> Arrays and pointers often look alike, but they have **fundamentally different roles**.
> An array *is* the memory; a pointer *points* to memory.
> Understand this distinction, and pointer arithmetic becomes intuitive — not mysterious.

---


