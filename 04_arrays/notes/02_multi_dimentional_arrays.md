# 🧩 Chapter 4 — Lesson 2: Two-Dimensional & Multidimensional Arrays

---

## 1️⃣ Overview

In C, **multi-dimensional arrays** extend the concept of one-dimensional arrays by adding extra indexing layers such as **rows** and **columns**.
A two-dimensional array (matrix) maps naturally to a **row-column grid**, while higher-dimensional arrays add more ranks (depth, time, etc.).

🧠 All of them still occupy **one contiguous block of memory** — laid out row-by-row in **row-major order**.

---

## 2️⃣ Two-Dimensional Arrays (2D)

### 2.1 Declaration & Initialization

```c
int matrix[2][3] = { {1, 2, 3}, {4, 5, 6} };
```

* 2 rows × 3 columns
* Total elements = 6
* Each `int` → 4 bytes (typically)

🧩 **Memory placement rule**
Row 0 → stored first, followed by Row 1, Row 2, … → row-major order.

---

### 2.2 Conceptual Layout (Grid View)

```
       Column →   0     1     2
Row ↓
 0               1     2     3
 1               4     5     6
```

### 2.3 Physical Layout (Memory View)

```
Address   Value   Element
-------   -----   ---------
100       1       matrix[0][0]
104       2       matrix[0][1]
108       3       matrix[0][2]
112       4       matrix[1][0]
116       5       matrix[1][1]
120       6       matrix[1][2]
```

📘 **Figure 4-2 — Two-Dimensional Array in Memory**

```
+------+------+------+------+------+------+ 
|100:1 |104:2 |108:3 |112:4 |116:5 |120:6 |
+------+------+------+------+------+------+
  [0][0] [0][1] [0][2] [1][0] [1][1] [1][2]
```

🧠 The array is **flattened** in memory — the compiler simply calculates each element’s address.

---

## 3️⃣ Address Calculation Formula

For a 2D array `matrix[R][C]`:

```
Address(matrix[i][j]) =
BaseAddress + (i * C + j) * sizeof(element)
```

Example:

```
Base = 100
C = 3
sizeof(int) = 4
Address(matrix[1][2]) = 100 + (1*3 + 2)*4 = 100 + 20 = 120
```

✅ Matches the memory map above.

---

## 4️⃣ Array of Arrays Concept

A 2D array is actually an **array of arrays**.

When you access `matrix[i]`, you get a **pointer to the i-th row**:

```c
for (int i = 0; i < 2; i++) {
    printf("&matrix[%d]: %p  sizeof(matrix[%d]): %zu\n",
           i, &matrix[i], i, sizeof(matrix[i]));
}
```

**Output example (assuming base = 100):**

```
&matrix[0]: 100  sizeof(matrix[0]): 12
&matrix[1]: 112  sizeof(matrix[1]): 12
```

🧠 Each row has 3 elements × 4 bytes = 12 bytes.

---

📘 **Memory Layout — Array of Arrays**

```
matrix (base) → [ &matrix[0], &matrix[1] ]

matrix[0] → 100 → {1, 2, 3}
matrix[1] → 112 → {4, 5, 6}
```

---

## 5️⃣ Multidimensional Arrays (3D and Beyond)

A **multidimensional array** extends this same pattern to more axes.
For instance, a 3D array can represent layers (depths) of 2D matrices.

```c
int arr3d[3][2][4] = {
    {{1, 2, 3, 4},   {5, 6, 7, 8}},
    {{9, 10, 11, 12}, {13, 14, 15, 16}},
    {{17, 18, 19, 20}, {21, 22, 23, 24}}
};
```

🧠 Terminology:

* **Row** → first index
* **Column** → second index
* **Rank** → third dimension (depth)

---

### 5.1 Memory Order (3D Row-Major)

The layout follows **row-column-rank** order:

```
arr3d[0][0][0]  → 100 → 1
arr3d[0][0][1]  → 104 → 2
arr3d[0][0][2]  → 108 → 3
arr3d[0][0][3]  → 112 → 4
arr3d[0][1][0]  → 116 → 5
arr3d[0][1][1]  → 120 → 6
...
arr3d[2][1][3]  → 192 → 24
```

📘 **Figure 4-3 — Three-Dimensional Array**

```
+-----------------------------------------------------+
| 1 | 2 | 3 | 4 | 5 | 6 | 7 | ... | 23 | 24 |        |
+-----------------------------------------------------+
  [0][0][0] → [2][1][3]
```

---

## 6️⃣ Address Calculation Formula (3D)

For an array `A[D1][D2][D3]`, element `A[i][j][k]` is at:

```
Address(A[i][j][k]) =
Base + ((i * D2 * D3) + (j * D3) + k) * sizeof(element)
```

Example:

```
Base = 100
sizeof(int) = 4
D2 = 2, D3 = 4
Address(A[2][1][3]) = 100 + ((2*2*4)+(1*4)+3)*4
                    = 100 + ((16+4+3)*4) = 100 + 92 = 192
```

✅ Matches the book’s diagram.

---

## 7️⃣ Understanding Pointers in Multidimensional Arrays

| Expression         | Meaning                                 |
| :----------------- | :-------------------------------------- |
| `matrix`           | Address of the first row (`&matrix[0]`) |
| `matrix[i]`        | Pointer to the i-th row                 |
| `matrix[i][j]`     | The actual element value                |
| `*(matrix[i] + j)` | Equivalent to `matrix[i][j]`            |

🧠 A 3D array works the same way, but each dimension adds another level of pointer indirection.

---

📘 **Pointer Relationship Diagram**

```
matrix
  ↓
  +-------------------+
  | &matrix[0]        |----> {1,2,3}
  | &matrix[1]        |----> {4,5,6}
  +-------------------+
```

In 3D:

```
arr3d
  ↓
  +---> &arr3d[0]
          ↓
          +---> &arr3d[0][0]
                 ↓
                 +---> arr3d[0][0][0] (value)
```

---

## 8️⃣ Practical Tips

| Tip                                                          | Why It Matters                                                      |
| :----------------------------------------------------------- | :------------------------------------------------------------------ |
| Keep track of dimension sizes                                | The compiler does not store them automatically                      |
| Use `sizeof` carefully                                       | Works only on arrays in the same scope (not after decay to pointer) |
| Don’t pass multi-dimensional arrays without specifying sizes | Function needs column dimension to compute addresses                |
| Avoid “jagged arrays” unless dynamically allocated           | C does not support non-rectangular static arrays                    |

---

## 9️⃣ Summary

| Concept                  | Description                                       |
| :----------------------- | :------------------------------------------------ |
| **2D array**             | Array of arrays (rows × columns)                  |
| **Row-major storage**    | Rows are stored sequentially in memory            |
| **3D array**             | Extends concept to layers (depth or rank)         |
| **Address formula**      | Calculates each element position from its indices |
| **Pointer relationship** | Each dimension adds a level of indirection        |

---

### 🧠 Key Takeaway

> Multi-dimensional arrays in C are just **flat contiguous memory** viewed through multiple indices.
> Understanding how the compiler **maps indices to addresses** gives you precise control over data layout, cache performance, and pointer manipulation.

---

