# ECE 312: Lab 1 — Dynamic Array  - Due Monday February 2nd @ 11:59pm

**Course:** ECE 312 – Software Design and Implementation I  
**Instructor:** Evan Speight 
**Topic:** Functions, Scope, Pointers, and Dynamic Memory Management

---

## Overview

In this lab, you will implement a **dynamic array** data structure in C. A dynamic array is similar to a standard array but can grow automatically when it runs out of space. This is the foundation of structures like C++'s `std::vector` and Java's `ArrayList`.

This lab is designed to give you hands-on experience with four fundamental C programming concepts:

1. **Functions** — Organizing code into reusable, modular units
2. **Scope** — Understanding where variables exist and how long they live
3. **Pointers** — Working with memory addresses and indirect access
4. **Dynamic Memory Management** — Allocating and freeing memory at runtime

---

## Learning Objectives

By completing this lab, you will be able to:

- Design and implement functions with various parameter types and return values
- Understand the difference between stack and heap memory
- Use pointers to pass data by reference and modify values through indirection
- Allocate memory dynamically using `malloc` and `realloc`
- Free memory properly using `free` to prevent memory leaks
- Handle error cases gracefully (NULL pointers, invalid inputs)

---

## Background

### Why Dynamic Arrays?

Standard C arrays have a fixed size determined at compile time:

```c
int arr[100];  // Always 100 elements, no more, no less
```

But what if you don't know how many elements you'll need? Dynamic arrays solve this by:

1. Allocating memory on the **heap** (not the stack)
2. Keeping track of both the current **size** (elements stored) and **capacity** (space available)
3. **Growing** automatically when more space is needed

### The DynamicArray Structure

You will work with this structure:

```c
typedef struct {
    int* data;      // Pointer to heap-allocated array of integers
    int size;       // Number of elements currently stored
    int capacity;   // Total slots available
} DynamicArray;
```

**Key insight:** The struct itself can be on the heap OR the stack, but `data` always points to heap memory.

### Memory Layout Visualization

```
Stack                          Heap
┌─────────────┐               ┌──────────────────────────────┐
│ arr (ptr)   │──────────────▶│ DynamicArray struct          │
└─────────────┘               │   data ───────┐              │
                              │   size: 3     │              │
                              │   capacity: 4 │              │
                              └───────────────│──────────────┘
                                              │
                                              ▼
                              ┌───┬───┬───┬───┐
                              │10 │20 │30 │ ? │  (int array)
                              └───┴───┴───┴───┘
                               [0] [1] [2] [3]
```

---

## Concepts in Practice

### Functions

Each operation on your dynamic array is encapsulated in its own function. This provides:

- **Modularity** — Each function does one thing well
- **Reusability** — Create arrays anywhere with `createArray()`
- **Abstraction** — Users don't need to know implementation details

Notice how different functions serve different purposes:

| Function | Purpose | Returns |
|----------|---------|---------|
| `createArray` | Constructor — allocates and initializes | Pointer to new array |
| `destroyArray` | Destructor — frees all memory | Nothing (void) |
| `addElement` | Mutator — modifies the array | Status code (0 or -1) |
| `getElement` | Accessor — retrieves data | Status code; value via pointer |

### Scope

Understanding scope is critical for this lab:

```c
DynamicArray* createArray(int initialCapacity) {
    DynamicArray* arr = malloc(sizeof(DynamicArray));  // arr is LOCAL
    arr->data = malloc(sizeof(int) * initialCapacity); // data is on HEAP
    arr->size = 0;                                      // size is in HEAP (part of struct)
    return arr;  // We can return arr because the MEMORY is on the heap
}                // arr (the pointer variable) goes away, but the memory remains
```

**Critical understanding:**
- Local variables (on the stack) disappear when a function returns
- Heap memory persists until explicitly freed
- Returning a pointer to heap memory is safe; returning a pointer to a local variable is **undefined behavior**

### Pointers

This lab uses pointers in several ways:

1. **Passing structs efficiently:**
   ```c
   int addElement(DynamicArray* arr, int value);  // Pass pointer, not copy
   ```

2. **Output parameters:**
   ```c
   int getElement(DynamicArray* arr, int index, int* result);
   // 'result' is where we WRITE the answer
   ```

3. **Accessing heap memory:**
   ```c
   arr->data[index] = value;  // arr-> dereferences, then we index
   ```

### Dynamic Memory Management

The three key functions:

| Function | Purpose | Notes |
|----------|---------|-------|
| `malloc(size)` | Allocate `size` bytes | Returns NULL on failure |
| `realloc(ptr, size)` | Resize allocation | May move data to new location |
| `free(ptr)` | Release memory | Don't use pointer afterward |

**The Golden Rule:** Every `malloc` must have a matching `free`.

**Common mistakes to avoid:**
- Memory leaks (forgetting to free)
- Double-free (freeing the same memory twice)
- Use-after-free (accessing memory after freeing it)
- Freeing in the wrong order (see `destroyArray`)

---

## Assignment

### Files Provided

| File | Description |
|------|-------------|
| `README.md` | This file.  |
| `dynarray.c` | Starter file — implement your functions here |
| `dynarray_test.c` | Sample tests to verify your implementation |


### File NOT Provided

| File | Description |
|------|-------------|
| `dynarray.h` | Header file with structure and function declarations. You decide what goes in here. |


### Functions to Implement

Implement all functions in `dynarray.c`. Detailed specifications are in the starter file comments.

#### 1. `createArray`

```c
DynamicArray* createArray(int initialCapacity);
```

Creates a new dynamic array with the specified initial capacity.

**Requirements:**
- Allocate memory for the `DynamicArray` struct and internal data array
- Initialize `size` and  `capacity`
- Return NULL if the function fails in any way (what ways could it fail?)

**Memory allocation order matters:**
```c
// If struct allocation succeeds but data allocation fails,
// you must free the struct before returning NULL
```

---

#### 2. `destroyArray`

```c
void destroyArray(DynamicArray* arr);
```

Frees all memory associated with the dynamic array.

**Requirements:**
- Free the memory allocated  (both the struct and the data)
- Handle NULL input gracefully (do nothing)

**Critical:** The order of `free` calls matters! Why?

---

#### 3. `addElement`

```c
int addElement(DynamicArray* arr, int value);
```

Adds an element to the end of the array, growing if necessary.

**Requirements:**
- Double the capacity using `realloc` if needed
- Manage the `size` variable correctly
- Return 0 on success, -1 on failure

**HINT: Using realloc correctly:**

```c
// WRONG - data loss risk:
arr->data = realloc(arr->data, newSize);  // If realloc fails, original pointer is lost!

// CORRECT:
int* temp = realloc(arr->data, newSize);
if (temp == NULL) {
    return -1;  // Original arr->data is still valid
}
arr->data = temp;  // Only update after confirming success
```

---

#### 4. `getElement`

```c
int getElement(DynamicArray* arr, int index, int* result);
```

Retrieves the element at the specified index.

**Requirements:**
- Check that `index` is valid (what does that mean?) 
- Store the value in `*result` (output parameter)
- Return 0 on success, -1 on failure
- Handle NULL pointers

**Output parameter pattern:**
```c
int value;
if (getElement(arr, 0, &value) == 0) {
    printf("Element is %d\n", value);
}
```

---

#### 5. `setElement`

```c
int setElement(DynamicArray* arr, int index, int value);
```

Updates the element at the specified index.

**Requirements:**
- Only modify existing elements 
- Return 0 on success, -1 on failure

---

#### 6. `getSize` and `getCapacity`

```c
int getSize(DynamicArray* arr);
int getCapacity(DynamicArray* arr);
```

Return the current size and capacity, or -1 if `arr` is NULL.

---

#### 7. `removeElement`

```c
int removeElement(DynamicArray* arr, int index);
```

Removes the element at the specified index by shifting subsequent elements left.

**Example:**
```
Before: [10, 20, 30, 40], size=4
Remove index 1
After:  [10, 30, 40, ?], size=3
         ↑   ↑   ↑
         0   1   2   (new indices)
```

**Requirements:**
- Shift elements properly
- Manage `size` correctly
- Return 0 on success, -1 on failure

---

## Compiling and Testing

### Compile Your Code

```bash
gcc -Wall -Wextra -o dynarray_test dynarray.c dynarray_test.c
```

The `-Wall -Wextra` flags enable extra warnings that help catch bugs.

### Run Tests

```bash
./dynarray_test
```

### Check for Memory Leaks

Use Valgrind to detect memory issues (available on ECE Linux machines):

```bash
valgrind --leak-check=full ./dynarray_test
```

A clean run shows:
```
All heap blocks were freed -- no leaks are possible
```

---

## Testing Tips

The provided test file covers basic functionality, but **you should write additional tests**. Consider:

### Edge Cases to Test

- Creating an array with capacity 1
- Adding many elements (forcing multiple resizes)
- Removing all elements one by one
- Operations on an empty array
- Removing the first, middle, and last elements
- `get`/`set` at boundary indices

### Example Custom Test

```c
void test_remove_all_elements() {
    DynamicArray* arr = createArray(3);
    addElement(arr, 1);
    addElement(arr, 2);
    addElement(arr, 3);
    
    // Remove all elements from the end
    while (getSize(arr) > 0) {
        removeElement(arr, getSize(arr) - 1);
    }
    
    if (getSize(arr) == 0) {
        printf("test_remove_all_elements PASSED\n");
    } else {
        printf("test_remove_all_elements FAILED\n");
    }
    
    destroyArray(arr);
}
```

---


## Submission Instructions

1. Complete all functions in `dynarray.c`
2. Test locally until all provided tests pass
3. Run Valgrind to check for memory leaks
4. Submit `dynarray.c` and `dynarray.h` to Gradescope
5. Review feedback and iterate

**Reminder:** This lab will have hidden test cases. Passing the visible tests does not guarantee full credit. Test thoroughly!

---

## Checklist

- [x] `createArray` allocates struct and data array, handles invalid input
- [x] `destroyArray` frees memory in correct order, handles NULL
- [x] `addElement` grows array when full, uses realloc safely
- [x] `getElement` validates bounds, uses output parameter correctly
- [x] `setElement` validates bounds, only modifies existing elements
- [x] `getSize` and `getCapacity` handle NULL input
- [x] `removeElement` shifts elements correctly, updates size
- [x] All functions handle NULL array input
- [x] No memory leaks (verified with Valgrind)
- [x] Code compiles without warnings using `-Wall -Wextra`
- [x] Name and EID filled out in your submitted source code 

---

## Summary

This lab reinforces four critical C concepts:

| Concept | How It Appears |
|---------|----------------|
| **Functions** | Modular design with clear responsibilities |
| **Scope** | Local vs. heap-allocated data lifetime |
| **Pointers** | Passing by reference, output parameters, indirection |
| **Dynamic Memory** | `malloc`, `realloc`, `free`, avoiding leaks |

Understanding these concepts is essential for the rest of ECE 312 and your career as a software developer. Take your time, test thoroughly, and don't hesitate to ask questions!

Good luck!
