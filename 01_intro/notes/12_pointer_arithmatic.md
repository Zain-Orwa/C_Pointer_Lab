````markdown
# 🧮 Pointer Arithmetic in C — Adding Integers to Pointers

---

## 🧠 1️⃣ Concept Overview

When you **add an integer to a pointer**, the result depends on the **size of the data type** that the pointer refers to.

Pointer arithmetic moves by **`n × sizeof(type)`** bytes.

For example:

```c
int *p;
p + 1;  // Moves ahead by sizeof(int) bytes
````

---

## ⚙️ 2️⃣ Why It Works This Way

A pointer is “typed” — the compiler knows what type it points to,
so it automatically scales the arithmetic based on the type’s size.

### Formula:

```
new_address = old_address + (n * sizeof(pointed_type))
```

---

## 📏 3️⃣ Common Data Type Sizes

| Data Type | Typical Size (Bytes) |
| :-------- | :------------------: |
| byte      |           1          |
| char      |           1          |
| short     |           2          |
| int       |           4          |
| long      |           8          |
| float     |           4          |
| double    |           8          |

*(These sizes vary between systems and compilers.)*

---

## 🧩 4️⃣ Example — Integer Pointer Arithmetic

```c
#include <stdio.h>

int main(void) {
    int vector[] = {28, 41, 7};
    int *pi = vector;  // pi → vector[0]

    printf("%d\n", *pi); // 28
    pi += 1;             // Move 4 bytes forward (int = 4 bytes)
    printf("%d\n", *pi); // 41
    pi += 1;             // Move another 4 bytes
    printf("%d\n", *pi); // 7

    return 0;
}
```

---

### 🧠 Memory Visualization

```text
vector: [28] [41] [7]
Address: 100  104  108
pi:      ↑
```

| Operation | Pointer Address | Dereferenced Value |
| :-------- | :-------------- | :----------------- |
| Start     | 100             | 28                 |
| `pi += 1` | 104             | 41                 |
| `pi += 1` | 108             | 7                  |

---

## 🧱 5️⃣ Adding Beyond the Array

```c
pi = vector;
pi += 3;  // moves past the array end → dangerous!
```

Now `pi` points **beyond valid memory** — an invalid or meaningless address.
Dereferencing it (`*pi`) would cause **undefined behavior**.

💡 **Rule:** Only perform pointer arithmetic within the same allocated object or array.

---

## 🧩 6️⃣ Example — `short` and `char` Pointers

```c
#include <stdio.h>

int main(void) {
    short s;
    short *ps = &s;
    char c;
    char *pc = &c;

    printf("ps before: %p\n", (void*)ps);
    ps = ps + 1;
    printf("ps after : %p\n\n", (void*)ps);

    printf("pc before: %p\n", (void*)pc);
    pc = pc + 1;
    printf("pc after : %p\n", (void*)pc);
}
```

### Example Output

```
ps before: 0x120
ps after : 0x122
pc before: 0x128
pc after : 0x129
```

| Pointer | Type      | Increment | Bytes Moved |
| :------ | :-------- | :-------- | :---------- |
| `ps`    | `short *` | `+1`      | 2 bytes     |
| `pc`    | `char *`  | `+1`      | 1 byte      |

💡 Pointers scale automatically depending on their **type size**.

---

## ⚠️ 7️⃣ Pointers to `void` and Arithmetic

A `void *` pointer has **no defined type size**, so pointer arithmetic on it is **not part of standard C**.
Some compilers (like GCC) allow it as an extension, but they issue a **warning**.

### Example

```c
#include <stdio.h>

int main(void) {
    int num = 5;
    void *pv = &num;

    printf("%p\n", pv);
    pv = pv + 1;  // ❌ Warning: pointer of type 'void *' used in arithmetic
}
```

### Typical Warning

```
warning: pointer of type 'void *' used in arithmetic [-Wpointer-arith]
```

Even though the compiler warns, some implementations (like GCC) still increase the pointer by `sizeof(void*)` (commonly 4 or 8 bytes).

---

## 🧱 8️⃣ Why This Matters

* Pointer arithmetic works **in units of the pointed-to type**, not raw bytes.
* You can safely move within an array or allocated block.
* Going outside that range → **undefined behavior**.
* Arithmetic on `void *` → **non-standard**, compiler-dependent.
* Arithmetic on function pointers → **illegal**.

---

## 🧠 9️⃣ Summary Visualization

```text
Address progression per +1 increment
────────────────────────────────────
char*   : +1 byte
short*  : +2 bytes
int*    : +4 bytes
double* : +8 bytes
void*   : ⚠️ Not allowed (undefined behavior)
```

---

## 🧩 🔟 Summary Table

| Pointer Type | Increment by 1 Adds | Notes                    |
| :----------- | :------------------ | :----------------------- |
| `char *`     | +1 byte             | 1 character              |
| `short *`    | +2 bytes            | 1 short value            |
| `int *`      | +4 bytes            | 1 integer                |
| `double *`   | +8 bytes            | 1 double precision float |
| `void *`     | ❌ Undefined         | No defined size          |

---

## 💬 11️⃣ Key Takeaways

> 🧠 Pointer arithmetic is **type-aware** — it moves in multiples of `sizeof(type)`.
> 🧠 Never perform arithmetic on a `void *` pointer (non-standard).
> 🧠 Avoid moving outside the allocated object — that’s undefined behavior.
> 🧠 Always use `%p` for printing addresses, and cast to `(void *)`.
> 🧠 Understanding this is **crucial for arrays, dynamic memory, and low-level programming**.

---

📘 **Figures referenced:**

* *Figure 1-7*: Memory allocation for integer vector
* *Figure 1-8*: Pointer arithmetic with `short` and `char`
* *Figure 1-9*: Pointer arithmetic warning on `void *`


# ➖ Subtracting and Comparing Pointers in C

---

## 🧠 1️⃣ Subtracting an Integer from a Pointer

Just like adding integers moves a pointer **forward**, subtracting integers moves it **backward** through memory.

The amount subtracted is:
```

integer × sizeof(pointed_type)

````

So, if you subtract `1` from an `int *` on a 32-bit system, the pointer address decreases by 4 bytes (since `sizeof(int) = 4`).

---

### 🧩 Example — Moving Backwards Through an Array

```c
#include <stdio.h>

int main(void) {
    int vector[] = {28, 41, 7};
    int *pi = vector + 2;  // pi → vector[2], address 108

    printf("%d\n", *pi);  // 7
    pi--;                 // move back to vector[1] (104)
    printf("%d\n", *pi);  // 41
    pi--;                 // move back to vector[0] (100)
    printf("%d\n", *pi);  // 28

    return 0;
}
````

### 💻 Output

```
7
41
28
```

### 🧭 Memory Visualization

```text
Addresses:   100      104      108
Array:      [28]     [41]      [7]
               ↑        ↑        ↑
              pi--    pi--    (start)
```

| Operation | Pointer Address | Value |
| :-------- | :-------------- | :---- |
| Start     | 108             | 7     |
| `pi--`    | 104             | 41    |
| `pi--`    | 100             | 28    |

💡 Each decrement moves the pointer back by 4 bytes.

---

## 🧮 2️⃣ Subtracting Two Pointers

When two pointers (to the **same array**) are subtracted, the result is the **difference in the number of elements between them** — not the difference in raw bytes.

The result type is `ptrdiff_t` (a signed integer type from `<stddef.h>`).

---

### 🧱 Example — Subtracting Pointers

```c
#include <stdio.h>

int main(void) {
    int vector[] = {28, 41, 7};
    int *p0 = vector;     // address 100
    int *p1 = vector + 1; // address 104
    int *p2 = vector + 2; // address 108

    printf("p2 - p0 = %ld\n", p2 - p0);  // 2
    printf("p2 - p1 = %ld\n", p2 - p1);  // 1
    printf("p0 - p1 = %ld\n", p0 - p1);  // -1

    return 0;
}
```

### 🧩 Output

```
p2 - p0 = 2
p2 - p1 = 1
p0 - p1 = -1
```

---

### 🧠 Explanation

* The **difference** represents the number of **array elements** between the pointers.
* The **sign** shows direction:

  * Positive → the left pointer is further ahead in memory
  * Negative → the left pointer is before the right one

---

### 💡 Visual Example

```text
vector: [28] [41] [7]
Address: 100  104  108
```

| Expression | Meaning         | Result |
| :--------- | :-------------- | :----: |
| `p2 - p0`  | (108 - 100) / 4 |    2   |
| `p2 - p1`  | (108 - 104) / 4 |    1   |
| `p0 - p1`  | (100 - 104) / 4 |   -1   |

---

## ⚙️ 3️⃣ `ptrdiff_t` — Safe Pointer Difference Type

The C standard defines a specific type for pointer subtraction:

```c
#include <stddef.h>
ptrdiff_t diff = p2 - p0;
```

✅ `ptrdiff_t` automatically adjusts to the system’s pointer size (32-bit, 64-bit, etc.),
making pointer arithmetic **portable and safe**.

---

## 🧩 4️⃣ Subtracting Dereferenced Pointers (Values)

Be careful — subtracting the **pointers themselves** is not the same as subtracting the **values they point to**.

```c
printf("*p0 - *p1 = %d\n", *p0 - *p1);  // 28 - 41 = -13
```

| Expression  | Meaning                             | Result |
| :---------- | :---------------------------------- | :----: |
| `p2 - p0`   | Pointer difference (elements apart) |   `2`  |
| `*p0 - *p1` | Value difference                    |  `-13` |

---

## 🔁 5️⃣ Comparing Pointers

Pointers can be compared using **relational operators** (`<`, `>`, `==`, `!=`, etc.).

This is useful only for pointers within the **same array or allocated block**, since outside comparisons are undefined.

---

### 🧱 Example — Pointer Comparison

```c
#include <stdio.h>

int main(void) {
    int vector[] = {28, 41, 7};
    int *p0 = vector;
    int *p1 = vector + 1;
    int *p2 = vector + 2;

    printf("p2 > p0: %d\n", p2 > p0);  // 1 (true)
    printf("p2 < p0: %d\n", p2 < p0);  // 0 (false)
    printf("p0 > p1: %d\n", p0 > p1);  // 0 (false)

    return 0;
}
```

### 🧭 Output

```
p2 > p0: 1
p2 < p0: 0
p0 > p1: 0
```

✅ `1` means **true**, `0` means **false**.

---

### 🧠 Notes

* Comparisons are valid only within the same contiguous memory region (array or block).
* You can safely use:

  * `==`, `!=` → check if two pointers refer to the same object
  * `<`, `>`, `<=`, `>=` → determine relative order in memory (within same array)
* Comparing pointers to unrelated objects → **undefined behavior**

---

## 🧩 6️⃣ Summary Visualization

```text
vector[] = { 28, 41, 7 }

Address:    100    104    108
Pointer:    p0     p1     p2
Value:      28     41     7
```

| Operation   | Result | Description              |
| :---------- | :----: | :----------------------- |
| `p2 - p0`   |   `2`  | 2 elements apart         |
| `p1 - p0`   |   `1`  | 1 element apart          |
| `p0 - p1`   |  `-1`  | p0 is before p1          |
| `*p0 - *p1` |  `-13` | Value difference         |
| `p2 > p0`   |   `1`  | p2 is after p0 in memory |
| `p0 > p1`   |   `0`  | p0 is before p1          |

---

## ⚠️ 7️⃣ Important Rules

> * 🧩 Pointer subtraction and comparison are only valid **within the same array or allocated block**.
> * 🧩 `ptrdiff_t` is the correct type to store pointer differences.
> * 🧩 Subtracting integers moves the pointer backward by multiples of `sizeof(type)`.
> * 🧩 Subtracting dereferenced pointers affects **values**, not addresses.
> * 🧩 Comparing unrelated pointers → undefined behavior.

---

💬 **In short:**

> ➕ Adding integers to pointers moves forward
> ➖ Subtracting integers moves backward
> 🔢 Subtracting pointers gives element distance
> ⚖️ Comparing pointers determines order
> 🧠 All valid only within the same memory region.

```

