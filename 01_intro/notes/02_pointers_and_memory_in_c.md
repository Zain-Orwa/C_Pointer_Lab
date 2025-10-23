### 🧠 *Pointers and Memory*

| 🧩 Memory Type | 📦 Lifetime | 🌍 Scope | 🧾 Description | Notes |
|:--|:--|:--|:--|:--|
| **Static / Global** | Entire program execution | Global or file-level | Variables declared with `static` or at global scope are stored here. They are created when the program starts and destroyed only when it ends. All functions can access global variables, but `static` variables are visible only within the file or function that defines them. |  |
| **Automatic (Stack)** | While the function is executing | Local to function | Variables declared inside a function (without `static`) live in stack memory. They are created when the function is called and destroyed when it returns. Their addresses become invalid afterward — pointers to them become **dangling pointers**. |  |
| **Dynamic (Heap)** | Until explicitly freed | Accessible via pointers | Memory allocated with `malloc()`, `calloc()`, or `realloc()` comes from the heap. It remains valid until `free()` is called. The pointer itself can be copied or passed around, but the memory it points to must be manually released. |

### ⚙️ Summary Rules

- 🧠 **Static/Global:** Allocated once → freed automatically at program exit.  
- ⚙️ **Automatic (Stack):** Allocated per function call → freed when function returns.  
- 🔧 **Dynamic (Heap):** Allocated manually → freed manually via `free()`.

---

### 💡 Common Pointer Implications

- Returning the address of a **stack variable** causes **undefined behavior** (the memory no longer exists).  
- Forgetting to `free()` heap memory causes a **memory leak**.  
- Global/static variables persist, so pointers to them remain valid throughout the program.

---

### 🧪 Suggested Experiment — `experiments/memory_regions.c`

```c
#include <stdio.h>
#include <stdlib.h>

int global_var = 10;

int* make_stack_pointer(void) {
    int local_var = 20;
    return &local_var; // ❌ Dangling pointer after return
}

int main(void) {
    static int static_var = 30;
    int *heap_ptr = malloc(sizeof(int));
    *heap_ptr = 40;

    printf("&global_var = %p\n", (void*)&global_var);
    printf("&static_var = %p\n", (void*)&static_var);
    printf("heap_ptr (heap) = %p\n", (void*)heap_ptr);
    printf("make_stack_pointer() = %p (invalid after return!)\n",
           (void*)make_stack_pointer());

    free(heap_ptr);
    return 0;
}
````

Run it:

```bash
make run CH=01_intro
```

📊 Observe:

* Global and static addresses are close together.
* Heap addresses usually higher (allocated dynamically).
* Stack addresses vary by call depth and grow *downward* on most systems.

---

```
## 📘 Table 1-1. Scope and Lifetime

| **Type**             | **Scope**                                      | **Lifetime**                          |
|----------------------|------------------------------------------------|----------------------------------------|
| **Global**           | The entire file                                | The lifetime of the application        |
| **Static**           | The function it is declared within             | The lifetime of the application        |
| **Automatic (local)**| The function it is declared within             | While the function is executing        |
| **Dynamic**          | Determined by the pointers that reference it   | Until the memory is freed              |


---

**Static vs. Automatic Variables**

* **Static Variable:**

  * Scope: Only visible inside the function where it’s declared.
  * Lifetime: Remains alive for the entire runtime of the program. Keeps its value between function calls.

* **Automatic (Local) Variable:**

  * Scope: Only visible inside the function where it’s declared.
  * Lifetime: Exists only while the function is executing, and is destroyed when the function ends.

**In Short:** Static variables remember their value between calls and live as long as the program runs. Automatic variables vanish after the function finishes.

``````
