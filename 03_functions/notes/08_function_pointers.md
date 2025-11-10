# 🧩 Chapter 3 — Lesson 8: Function Pointers

---

## 1️⃣ Overview

A **function pointer** stores the **address of a function** instead of normal data.
It allows calling functions **indirectly**, selecting logic at runtime, and designing highly flexible systems.

🧠 **Use cases:**

* Callbacks and event systems
* Lookup / dispatch tables
* Replacing long `if` or `switch` chains
* Modular design (e.g., interpreters, workflows)

---

## 2️⃣ Declaring a Function Pointer

A pointer to a function that takes no arguments and returns nothing:

```c
void (*foo)();
```

| Part   | Meaning                  |
| :----- | :----------------------- |
| `foo`  | variable name            |
| `*foo` | it’s a pointer           |
| `()`   | points to a function     |
| `void` | function returns nothing |

---

📘 **Memory Layout — Declaration**

```
CODE SEGMENT (.text)
---------------------
| function(s)...     |
| ...                |
|                    |
DATA / STACK SEGMENT
---------------------
| foo (pointer) -------> points to function code
```

🧠 Function code lives in **.text**, while `foo` is stored in **RAM** (stack or data section).

---

## 3️⃣ Example: Using a Function Pointer

```c
int square(int num2) {
    return num2 * num2;
}

int (*fptr1)(int);
fptr1 = square;

printf("%d squared is %d\n", 5, fptr1(5));
```

🧠 The function pointer holds the **address of `square()`** and executes it indirectly.

---

📘 **Memory Layout — Using Function Pointer**

```
CODE (.text)
---------------------------------------------------
| square(): instructions                         |
---------------------------------------------------

STACK / DATA
---------------------------------------------------
| num2 = 5                                        |
| fptr1 = &square() ------------------------------+
+-------------------------------------------------+
                          |
                          v
                   Calls square(5) → returns 25
```

---

## 4️⃣ Typedef for Function Pointers

Simplify syntax using a type alias:

```c
typedef int (*funcptr)(int);

funcptr fptr2 = square;
printf("%d squared is %d\n", 8, fptr2(8));
```

🧠 This is much cleaner when sharing function pointer types across modules.

---

📘 **Memory Layout — Typedef Version**

```
CODE (.text)
---------------------------------------------------
| square()                                        |
---------------------------------------------------

STACK / DATA
---------------------------------------------------
| num2  = 8                                       |
| fptr2 = &square() ------------------------------+
+-------------------------------------------------+
                          |
                          v
                 Executes square(8) → returns 64
```

---

## 5️⃣ Passing Function Pointers (Callbacks)

We can pass a function pointer as an argument — this is how **callbacks** work.

```c
int add(int a, int b)  { return a + b; }
int sub(int a, int b)  { return a - b; }

typedef int (*fptrOperation)(int, int);

int compute(fptrOperation operation, int num2, int num3) {
    return operation(num2, num3);
}

printf("%d\n", compute(add, 5, 6)); // 11
printf("%d\n", compute(sub, 5, 6)); // -1
```

🧠 The `compute()` function doesn’t care what operation it runs — it just executes the pointer it’s given.

---

📘 **Memory Layout — Passing Function Pointer**

```
CODE (.text)
---------------------------------------------------
| add()                                           |
| sub()                                           |
| compute()                                       |
---------------------------------------------------

STACK (for compute)
---------------------------------------------------
| num2       = 5                                  |
| num3       = 6                                  |
| operation  = &add() -----------------------------+
+-------------------------------------------------+
                          |
                          v
                   executes add(5,6) → returns 11
```

---

## 🧩 Deep Dive: Stack Frame Evolution in `compute(add, 5, 6)`

Here’s a step-by-step visualization of what happens when this line runs:

```c
compute(add, 5, 6);
```

---

### **Step 1 — Main Before the Call**

```
STACK
---------------------------------------------------
| num2 = 5                                        |
| num3 = 6                                        |
| &add()   (address of function)                  |
---------------------------------------------------

CODE (.text)
---------------------------------------------------
| add()       | compute()     | main()           |
---------------------------------------------------
```

Main prepares arguments for the call — it pushes `5`, `6`, and the address of `add()`.

---

### **Step 2 — Entering `compute()`**

```
STACK
---------------------------------------------------
| return address to main()                       |
| operation = &add()                             |
| num2 = 5                                       |
| num3 = 6                                       |
---------------------------------------------------
CODE (.text)
---------------------------------------------------
| add(): function machine code                   |
| compute(): executing operation(num2, num3) ----+
---------------------------------------------------
```

`compute()` receives:

* a pointer to `add()`
* integers `5` and `6`

Then it calls `operation(num2, num3)` — which means jump to the address of `add()`.

---

### **Step 3 — Inside `add()`**

```
STACK
---------------------------------------------------
| return address to compute()                    |
| a = 5                                          |
| b = 6                                          |
---------------------------------------------------
CODE (.text)
---------------------------------------------------
| add(): executing return a + b -----------------+
---------------------------------------------------
```

`add()` executes and returns `11`.

---

### **Step 4 — Returning Back**

```
STACK unwinds...

compute() frame destroyed → returns 11 to main()
main() resumes with the result.
```

🧠 **Flow Summary:**

| Step | Action                          | Memory                    |
| :--- | :------------------------------ | :------------------------ |
| 1    | Main pushes args                | Stack grows               |
| 2    | compute() gets function pointer | Stack adds frame          |
| 3    | Jump to add()                   | Code executes via pointer |
| 4    | add() returns                   | Stack unwinds             |

✅ **Result:** `compute()` calls `add()` **without knowing it directly.**
The pointer made it happen.

---

## 6️⃣ Returning Function Pointers

Functions can also **return** a function pointer.

```c
fptrOperation select(char opcode) {
    switch (opcode) {
        case '+': return add;
        case '-': return sub;
    }
    return NULL;
}

int evaluate(char opcode, int num2, int num3) {
    fptrOperation op = select(opcode);
    return op(num2, num3);
}
```

---

📘 **Memory Layout — Returning Function Pointer**

```
CODE (.text)
---------------------------------------------------
| add()                                           |
| sub()                                           |
| select()                                        |
| evaluate()                                      |
---------------------------------------------------

STACK (during evaluate)
---------------------------------------------------
| opcode = '+'                                   |
| num2   = 5                                     |
| num3   = 6                                     |
| op = &add()  <--- returned from select()        |
+-------------------------------------------------+
                          |
                          v
                   executes add(5,6) → returns 11
```

---

## 7️⃣ Arrays of Function Pointers

Create a **dispatch table** for operations.

```c
typedef int (*operation)(int, int);
operation operations[128] = {NULL};

void initializeOperationsArray() {
    operations['+'] = add;
    operations['-'] = sub;
}

int evaluateArray(char opcode, int num2, int num3) {
    operation op = operations[opcode];
    return op(num2, num3);
}
```

---

📘 **Memory Layout — Array of Function Pointers**

```
CODE (.text)
---------------------------------------------------
| add()                                           |
| sub()                                           |
| evaluateArray()                                 |
---------------------------------------------------

DATA
---------------------------------------------------
| operations['+'] -> &add()                       |
| operations['-'] -> &sub()                       |
---------------------------------------------------

STACK
---------------------------------------------------
| opcode = '+'                                   |
| num2 = 5                                       |
| num3 = 6                                       |
| op = operations['+'] -> &add() -----------------+
+-------------------------------------------------+
                          |
                          v
                   executes add(5,6)
```

🧠 Like a **map of commands**, where each symbol leads to its corresponding logic.

---

## 8️⃣ Comparing Function Pointers

```c
fptrOperation fptr1 = add;

if (fptr1 == add)
    printf("fptr1 points to add\n");
```

📘 **Memory Layout**

```
CODE (.text)
---------------------------------------------------
| add()                                           |
---------------------------------------------------

STACK
---------------------------------------------------
| fptr1 = &add()                                 |
---------------------------------------------------

Comparison: (fptr1 == &add) → TRUE
```

---

## 9️⃣ Casting Function Pointers

Casting works, but use caution — parameter mismatches cause **undefined behavior**.

```c
typedef int (*fptrOne)(int);
typedef int (*fptrTwo)(int, int);

int add(int, int);

fptrTwo fptrFirst = add;
fptrOne fptrSecond = (fptrOne)fptrFirst;
fptrFirst = (fptrTwo)fptrSecond;

printf("%d\n", fptrFirst(5, 6)); // 11
```

📘 **Memory Layout — Casting**

```
CODE (.text)
---------------------------------------------------
| add()                                           |
---------------------------------------------------

STACK
---------------------------------------------------
| fptrFirst  = &add()                             |
| fptrSecond = &add() (after cast)                |
---------------------------------------------------
```

⚠️ Never cast to or from `void*`. Function pointers are **not guaranteed** to have the same representation as data pointers.

---

## 🔟 Base Function Pointer Type

Generic placeholder type:

```c
typedef void (*fptrBase)();
```

It can temporarily hold any function pointer — but must be cast back before calling.

---

## 1️⃣1️⃣ Summary

| Concept            | Description                    |
| :----------------- | :----------------------------- |
| Function pointer   | Stores a function’s address    |
| Typedef alias      | Makes code cleaner             |
| Passing pointers   | Enables callbacks              |
| Returning pointers | Enables runtime selection      |
| Array of pointers  | Replaces switch blocks         |
| Comparison         | Useful for checking identity   |
| Casting            | Risky — avoid unless necessary |

---

### 🧠 Key Takeaway

> Function pointers give **behavioral indirection** — not just data flexibility.
> They enable modular, event-driven systems, callback mechanisms, and dynamic decision logic.
> Once you understand them, you gain fine-grained control over execution flow and memory behavior.

---

