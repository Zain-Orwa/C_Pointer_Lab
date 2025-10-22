# Ch1 — Introduction (notes)

## 🧩 Types of “Nulls” in C

C supports multiple equivalent representations of a **null pointer**, each expressing  
“a pointer that does not refer to any valid memory location.”

| 🧠 Type | 🧾 Definition | 💻 Example | 🗒️ Notes |
|:--|:--|:--|:--|
| **Literal zero** | `0` | `int *p = 0;` | The oldest form — integer constant `0` auto-converted to a null pointer in pointer context. |
| **Macro `NULL`** | `((void*)0)` or `0` | `int *p = NULL;` | Standard symbolic constant defined in `<stddef.h>`, `<stdio.h>`, `<stdlib.h>`, etc. *(Preferred in pure C)* |
| **Explicit cast** | `(void*)0` | `int *p = (void*)0;` | Explicitly shows that it’s a pointer, not an integer. Useful in generic or low-level code. |
| **`nullptr` (C++)** | keyword (C++11+) | `int *p = nullptr;` | Type-safe null value. **Not valid in C**, only in C++. |

---

### ⚙️ Implementation Notes

- All of these forms **evaluate to a null pointer constant** — a symbolic “no-address.”  
- The actual **bit pattern** of a null pointer **is implementation-defined** (not always `0x00000000`).  
- Always use `NULL` or `(void*)0` for clarity and portability.  
- Comparing any valid pointer with `NULL` is safe; dereferencing it is **undefined behavior**.

---

### 🧪 Quick Demo Idea
Create an `experiments/null_types.c` file:

```c
#include <stdio.h>
#include <stddef.h>

int main(void) {
    int *p1 = 0;
    int *p2 = NULL;
    int *p3 = (void*)0;

    printf("p1=%p | p2=%p | p3=%p\n", (void*)p1, (void*)p2, (void*)p3);
    printf("All equal? %s\n", (p1 == p2 && p2 == p3) ? "YES" : "NO");
    return 0;
}
````

Run it:

```bash
make run CH=01_intro
```

Expected output:

```
p1=0x0 | p2=0x0 | p3=0x0
All equal? YES
```

---

