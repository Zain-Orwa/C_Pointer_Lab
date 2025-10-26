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

```
