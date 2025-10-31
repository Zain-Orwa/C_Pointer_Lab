````markdown
# 🧩 Chapter 3 — Lesson 3: Passing a Pointer to a Constant

---

## 1️⃣ Overview

Sometimes we want to **share access** to data with a function,  
but we **don’t want that function to modify** the original value.  

➡️ Solution: **Pointer to a constant (`const` pointer)**

This is one of the most important const-patterns in C.  
It protects data from accidental modification while still allowing efficient pointer access.

---

## 2️⃣ Why We Need “Pointers to Constants”

| Problem                                                              | What Happens                                    | Why This Is Dangerous                                            |
| :--------------------------------------------------------------------| :-----------------------------------------------| :----------------------------------------------------------------|
| A function receives a pointer to your variable                       | It can modify your original data directly.      | A small mistake can corrupt important values.                    |
| You want to share large data (e.g., configuration, read-only buffer) | Copying the entire data wastes time and memory. | You want to pass **address only** without allowing modification. |

🧠 **Analogy:**  
Think of a **pointer to a constant** as giving someone a **read-only key** to your house.  
They can look around, take notes — but cannot move or change anything inside.

---

## 3️⃣ Syntax Breakdown

| Type                 | Meaning                                                           |
| :--------------------| :-----------------------------------------------------------------|
| `int *p`             | Pointer to an integer (modifiable)                                |
| `const int *p`       | Pointer to a **constant integer** (the integer cannot be changed) |
| `int *const p`       | A **constant pointer** (the pointer itself cannot change)         |
| `const int *const p` | Both pointer and value are constant                               |

---

## 4️⃣ Example — Passing a Pointer to a Constant

Here’s a function that reads from one integer and writes to another:

```c
void passingAddressOfConstants(const int *num1, int *num2) {
    *num2 = *num1;
}
````

And in `main()`:

```c
int main() {
    const int limit = 100;
    int result = 5;

    passingAddressOfConstants(&limit, &result);
    return 0;
}
```

✅ **Explanation:**

| Variable | Type          | Role                                 |
| :------- | :------------ | :----------------------------------- |
| `limit`  | `const int`   | Read-only source                     |
| `result` | `int`         | Writable destination                 |
| `num1`   | `const int *` | Can read from but not modify `limit` |
| `num2`   | `int *`       | Can modify `result`                  |

🧠 **Why it works:**
We’re copying the *value* of `limit` into `result`,
but we’re not changing `limit` itself — it stays safe.

---

## 5️⃣ What Happens in Memory

### 📊 Stack Visualization

```
+---------------------------+
| passingAddressOfConstants |
|---------------------------|
| num1 → 0x500 (limit)      |
| num2 → 0x504 (result)     |
+---------------------------+
| main() Frame              |
| limit:  100  @ 0x500      |
| result: 5    @ 0x504      |
+---------------------------+
```

After executing `*num2 = *num1;`:

```
| limit:  100  @ 0x500      |
| result: 100  @ 0x504      | ← copied from limit
```

✅ **Result:** The function safely copies without modifying `limit`.

---

## 6️⃣ Attempting to Modify a Constant (Error Example)

Let’s see what happens if we try to modify both:

```c
void passingAddressOfConstants(const int *num1, int *num2) {
    *num1 = 100;  // ❌ ERROR — num1 points to constant data
    *num2 = 200;  // ✅ OK — num2 points to writable data
}
```

If you call this function with:

```c
const int limit = 100;
passingAddressOfConstants(&limit, &limit);
```

You’ll get a **compile-time error** because:

* The function expects the **second parameter** (`int *`) to point to writable data.
* But we passed the address of a **constant**, which cannot legally be modified.

---

### 🧠 Why the Compiler Complains

| Parameter | Expected Type | What You Passed    | Why Error                        |
| :-------- | :------------ | :----------------- | :------------------------------- |
| `num1`    | `const int *` | `&limit` (OK)      | You can read from it.            |
| `num2`    | `int *`       | `&limit` (❌ Wrong)| You cannot modify a `const int`. |

C protects you from accidentally **writing into constant memory**.

---

## 7️⃣ Forbidden Case — Taking Address of Literal

This will also fail:

```c
passingAddressOfConstants(&23, &23);  // ❌ Invalid
```

Because integer literals like `23` are **rvalues**, not **lvalues**.
They exist temporarily in the expression, not as variables in memory.

| Term       | Meaning                                                 |
| :--------- | :------------------------------------------------------ |
| **lvalue** | Object with a memory location (can take its address)    |
| **rvalue** | Temporary value without an address (can’t take address) |

🧠 **Analogy:**
An **lvalue** is a house — it has an address.
An **rvalue** is a cloud — it exists but you can’t take its address.

---

## 8️⃣ Summary Diagram

```
   ┌───────────────────────────────┐
   │ const int limit = 100         │
   │ int result = 5                │
   ├───────────────────────────────┤
   │ passingAddressOfConstants()   │
   │ num1 → limit (read only)      │
   │ num2 → result (modifiable)    │
   └───────────────────────────────┘
```

✅ `limit` stays unchanged
✅ `result` becomes `100`
🚫 You cannot do `*num1 = 50;` (compiler error)

---

## 9️⃣ Real-Life Analogy

| Action         | Analogy                                             | Explanation                                        |
| :------------- | :-------------------------------------------------- | :------------------------------------------------- |
| `int *p`       | You lend your car keys to a friend.                 | They can **drive and modify** the car.             |
| `const int *p` | You lend your car keys, but the ignition is locked. | They can **look** inside but not start or move it. |

---

## 🔟 Key Takeaways

| #  | Concept                 | Summary                                                       |
| :- | :---------------------- | :------------------------------------------------------------ |
| 1  | **Pointer to Constant** | Lets you pass data safely without allowing modification.      |
| 2  | **Const Protection**    | Compiler enforces read-only access.                           |
| 3  | **Why It Exists**       | Prevents accidental overwriting of important data.            |
| 4  | **Rvalue Restriction**  | You can’t take the address of a literal like `&23`.           |
| 5  | **Best Practice**       | Use `const` whenever a function should not change input data. |

---


```

