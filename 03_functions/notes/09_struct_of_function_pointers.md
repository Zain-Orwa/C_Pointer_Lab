---

## 1️⃣2️⃣ Function Pointers Inside Structures

So far, we’ve stored function pointers as **stand-alone variables** or arrays.
But in real programs — especially larger systems — function pointers are often stored **inside `struct`s**.
This allows you to group both **data** and **the behavior that acts on that data**, creating a small, flexible “object-like” unit.

---

### 🧩 Example 1 — Operation Table (from the Book Concept)

Let’s extend the earlier arithmetic example by embedding function pointers in a structure:

```c
typedef int (*fptrOperation)(int, int);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

typedef struct sOperation {
    char symbol;                 // '+', '-', '*', etc.
    fptrOperation perform;       // pointer to the actual function
} sOperation;

sOperation operations[] = {
    {'+', add},
    {'-', sub},
};

int evaluateStruct(char opcode, int num2, int num3) {
    for (int i = 0; i < 2; ++i) {
        if (operations[i].symbol == opcode)
            return operations[i].perform(num2, num3);
    }
    return 0;
}

printf("%d\n", evaluateStruct('+', 5, 6));  // 11
printf("%d\n", evaluateStruct('-', 5, 6));  // -1
```

---

📘 **Memory Layout — Struct With Function Pointer**

```
CODE (.text)
---------------------------------------------------
| add()                                           |
| sub()                                           |
| evaluateStruct()                                |
---------------------------------------------------

DATA
---------------------------------------------------
| operations[0]                                  |
|   symbol = '+'                                 |
|   perform = &add() -----------------------------+
| operations[1]                                  |
|   symbol = '-'                                 |
|   perform = &sub() -----------------------------+
---------------------------------------------------

STACK
---------------------------------------------------
| opcode = '+'                                   |
| num2   = 5                                     |
| num3   = 6                                     |
| i = 0                                          |
---------------------------------------------------
                          |
                          v
                  operations[0].perform(5,6)
```

🧠 **Explanation:**
Each structure instance acts like a “record” that pairs a **command** (`'+'`) with the **action** (`add()`’s address).
`evaluateStruct()` simply loops through the table and executes the right function.

---

### 🧩 Example 2 — Workflow System (Book-Inspired)

Imagine each function performs a **step** in a process — just like in the “workflow of operations” example in the book.

```c
#include <stdio.h>

typedef void (*WorkflowStep)(void *context);

typedef struct sWorkflow {
    const char *name;
    WorkflowStep steps[3];
} sWorkflow;

typedef struct sContext {
    int num2;
    int num3;
} sContext;

void step_add(void *ctx) {
    sContext *c = (sContext*)ctx;
    printf("Sum: %d\n", c->num2 + c->num3);
}

void step_sub(void *ctx) {
    sContext *c = (sContext*)ctx;
    printf("Difference: %d\n", c->num2 - c->num3);
}

void runWorkflow(sWorkflow *wf, void *ctx) {
    for (int i = 0; wf->steps[i] != NULL; ++i)
        wf->steps[i](ctx);
}

int main(void) {
    sWorkflow calc = {"Calculator", {step_add, step_sub, NULL}};
    sContext ctx = {10, 4};

    runWorkflow(&calc, &ctx);
    return 0;
}
```

🧩 **Output**

```
Sum: 14
Difference: 6
```

---

📘 **Memory Layout — Workflow Example**

```
CODE (.text)
---------------------------------------------------
| step_add()                                      |
| step_sub()                                      |
| runWorkflow()                                   |
| main()                                          |
---------------------------------------------------

DATA
---------------------------------------------------
| sWorkflow calc                                  |
|   name  = "Calculator"                          |
|   steps[0] = &step_add() -----------------------+
|   steps[1] = &step_sub() -----------------------+
|   steps[2] = NULL                               |
---------------------------------------------------

STACK
---------------------------------------------------
| sContext ctx                                   |
|   num2 = 10                                   |
|   num3 = 4                                    |
| wf = &calc                                    |
---------------------------------------------------
                          |
                          v
                   runWorkflow()
                        |
        +---------------+----------------+
        |                                |
  steps[0](ctx)                     steps[1](ctx)
 step_add(10,4)                     step_sub(10,4)
```

🧠 **Explanation:**

* Each struct stores its own “function pipeline.”
* You can **add**, **remove**, or **reorder** behavior without changing core logic.
* This is the same idea behind **function pointer arrays** from earlier, but now **encapsulated** in a structured type.

---

### 🧠 Analogy

Think of the structure like a **toolbox**:

* Data fields = the materials to work on (`num2`, `num3`).
* Function pointers = the tools (`step_add`, `step_sub`).
  You hand the toolbox to `runWorkflow()`, and it runs each tool on the data.

---

## 1️⃣3️⃣ Why Embed Function Pointers in Structs?

| Reason              | Benefit                                                                                   |
| :------------------ | :---------------------------------------------------------------------------------------- |
| **Modularity**      | Keeps logic related to a specific feature in one place.                                   |
| **Polymorphism**    | You can swap out behaviors by swapping function pointers — like method overriding in OOP. |
| **Maintainability** | Easy to extend without touching existing code.                                            |
| **Performance**     | No need for `if`/`switch` blocks; jumps directly via pointer.                             |

---

📘 **Memory Relationship Summary**

```
+-----------------------------------------------------------+
| STRUCT OBJECT (e.g., sWorkflow or sOperation)             |
|-----------------------------------------------------------|
| Data Field(s)    → num2, num3, symbol, etc.               |
| Function Pointer → points to code section (.text)         |
+-----------------------------------------------------------+

         |                             ^
         |                             |
         |-------> CODE SEGMENT (.text)-+
                    executes the actual logic
```

---

## 1️⃣4️⃣ Summary

| Concept                    | Description                                 |
| :------------------------- | :------------------------------------------ |
| Function pointer in struct | Binds behavior with data                    |
| `typedef` usage            | Simplifies pointer syntax                   |
| Workflow example           | Demonstrates real-world use                 |
| Benefit                    | Modularity, flexibility, clean architecture |

---

### 🧠 Final Key Takeaway

> Embedding function pointers inside structures is where **C starts feeling object-oriented**.
> You gain the ability to assign different behaviors to different instances — dynamically.
> This is the foundation of **callbacks**, **virtual function tables**, and even **polymorphism** in system-level code.

---

