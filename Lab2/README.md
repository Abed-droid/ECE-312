# ECE 312: Lab 2 Linked Lists - Due Feb 16 @ 11:59pm

**Course:** ECE 312 – Software Design and Implementation I  
**Instructor:** Evan Speight 
**Topic:** Linked Lists, Pointer Manipulation, and Dynamic Node Allocation

---

## Overview

In this lab, you will implement a **singly linked list** data structure in C. Unlike arrays, linked lists store elements in individual nodes scattered throughout memory, connected by pointers. This structure allows for efficient insertion and deletion at any position without shifting elements.

This lab builds on concepts from the previous Dynamic Array lab while introducing new challenges unique to node-based data structures.

---

## Learning Objectives

By completing this lab, you will be able to:

- Understand the structure of a linked list and how nodes are connected
- Implement common linked list operations (insert, remove, traverse)
- Manipulate pointers to rewire node connections
- Handle edge cases involving empty lists and boundary positions
- Properly allocate and free individual nodes to prevent memory leaks

---

## Background


Linked lists excel when you need frequent insertions/deletions at the beginning or when you don't know the size in advance.

### The Node Structure

Each node contains two things:

```c
typedef struct Node {
    int data;           // The value stored in this node
    struct Node* next;  // Pointer to the next node (or NULL if last)
} Node;
```

### The LinkedList Structure

The list itself just keeps track of where it starts:

```c
typedef struct {
    Node* head;   // Pointer to first node (NULL if empty)
    int size;     // Number of nodes in the list
} LinkedList;
```

### Memory Visualization

```
LinkedList struct           Node structs (scattered in heap)
┌─────────────┐
│ head ───────┼──────────▶ ┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│ size: 3     │            │ data: 10     │     │ data: 20     │     │ data: 30     │
└─────────────┘            │ next ────────┼────▶│ next ────────┼────▶│ next: NULL   │
                           └──────────────┘     └──────────────┘     └──────────────┘
```

### Key Concept: Pointer Rewiring

The heart of linked list manipulation is **changing where pointers point**. Consider inserting a new node:

```
Before: [A] ──────────▶ [C]

After:  [A] ──▶ [B] ──▶ [C]
```

To achieve this:
1. Create new node B
2. Set B's next to point to C (`newNode->next = A->next`)
3. Set A's next to point to B (`A->next = newNode`)

**Order matters!** If you do step 3 first, you lose access to C.

---

## Assignment

### Files Provided

| File | Description |
|------|-------------|
| `linkedlist.h`    | Empty Header file that you fill with structure and function declarations |
| `linkedlist.c` | Starter file — implement your functions here |
| `linkedlist_test.c` | Sample tests to verify your implementation |

### Functions to Implement

Implement all functions in `linkedlist.c`. 

---

#### 1. `createList`

```c
LinkedList* createList(void);
```

Creates a new empty linked list.

**Requirements:**
- Allocate memory for the `LinkedList` struct
- Initialize `head` to NULL and `size` to 0
- Return NULL if allocation fails

---

#### 2. `destroyList`

```c
void destroyList(LinkedList* list);
```

Frees all memory: every node and the list struct itself.

**Requirements:**
- Traverse the list, freeing each node
- Free the list struct
- Handle NULL input gracefully

**Critical Pattern — Saving the next pointer:**

Be sure you free in the right order so you don't lose access to the rest of the list!

---

#### 3. `insertAtHead`

```c
int insertAtHead(LinkedList* list, int value);
```

Inserts a new node at the beginning of the list.

**Requirements:**
- Allocate a new node
- Set the new node's next to the current head
- Update head to point to the new node
- Increment size
- Return 0 on success, -1 on failure

**Example:**
```
Before: head -> [20] -> [30] -> NULL
insertAtHead(list, 10)
After:  head -> [10] -> [20] -> [30] -> NULL
```

---

#### 4. `insertAtTail`

```c
int insertAtTail(LinkedList* list, int value);
```

Inserts a new node at the end of the list.

**Requirements:**
- Allocate a new node with next = NULL
- If list is empty, new node becomes head
- Otherwise, traverse to the last node and link the new node
- Increment size
- Return 0 on success, -1 on failure
- If list is empty, handle that case specially

**Example:**
```
Before: head -> [10] -> [20] -> NULL
insertAtTail(list, 30)
After:  head -> [10] -> [20] -> [30] -> NULL
```

---

#### 5. `insertAtIndex`

```c
int insertAtIndex(LinkedList* list, int index, int value);
```

Inserts a new node at the specified position.

**Valid indices:** 0 to size (inclusive)
- Index 0: insert at head
- Index size: insert at tail
- Anything in between: insert in middle

**Requirements:**
- Validate index bounds
- Handle special cases (index 0, index == size)
- For middle insertion: traverse to node at index-1, then rewire
- Return 0 on success, -1 on failure


---

#### 6. `removeAtHead`

```c
int removeAtHead(LinkedList* list);
```

Removes the first node from the list.

**Requirements:**
- Return -1 if list is empty
- Save pointer to current head
- Update head to point to second node
- Free the old head
- Decrement size
- Return 0 on success

**Example:**
```
Before: head -> [10] -> [20] -> [30] -> NULL
removeAtHead(list)
After:  head -> [20] -> [30] -> NULL
```

---

#### 7. `removeAtIndex`

```c
int removeAtIndex(LinkedList* list, int index);
```

Removes the node at the specified position.

**Valid indices:** 0 to size-1

**Requirements:**
- Validate index bounds
- Handle special case (index 0)
- For middle removal: traverse to node at index-1, then rewire and free
- Decrement size
- Return 0 on success, -1 on failure


---

#### 8. `getElement`

```c
int getElement(LinkedList* list, int index, int* result);
```

Retrieves the value at the specified index.

**Requirements:**
- Validate index bounds (0 to size-1)
- Traverse to the node at that index
- Store the value in `*result`
- Return 0 on success, -1 on failure

---

#### 9. `getSize`

```c
int getSize(LinkedList* list);
```

Returns the number of elements in the list, or -1 if list is NULL.

---

## Common Mistakes

### 1. Losing the rest of the list


### 2. Forgetting empty list case


### 3. Off-by-one in traversal


### 4. Freeing before saving next


### 5. Not handling NULL inputs

---

## Compiling and Testing

### Compile Your Code

```bash
gcc -Wall -Wextra -o linkedlist_test linkedlist.c linkedlist_test.c
```

### Run Tests

```bash
./linkedlist_test
```

### Check for Memory Leaks

```bash
valgrind --leak-check=full ./a
```
**Expected output for a correct implementation:**
```
All heap blocks were freed -- no leaks are possible
```

or use a memory sanitizer:

```bash
gcc -fsanitize=address -Wall -Wextra -o linkedlist_test linkedlist.c linkedlist_test.c
./linkedlist_test


---

## Testing Tips

Write additional tests for these scenarios at a minimum:

- Insert into empty list (all three insert functions)
- Remove from single-element list
- Remove the last element
- Multiple consecutive operations
- Operations after emptying and refilling the list

### Debugging Helper

The test file includes a `printList()` function. Use it to visualize your list state:

```c
printList(list);  // Output: List (size=3): head -> [10] -> [20] -> [30] -> NULL
```

---

## Submission Instructions

1. Complete all functions in `linkedlist.c`
2. Test locally until all provided tests pass
3. Run Valgrind to check for memory leaks
4. Submit your code  to Gradescope
5. Review feedback and iterate

**Reminder:** The autograder includes hidden test cases. Passing the visible tests does not guarantee full credit. Test edge cases thoroughly!

---

## Checklist

- [ ] `createList` allocates and initializes correctly
- [ ] `destroyList` frees all nodes AND the list struct, handles NULL
- [ ] `insertAtHead` works on empty and non-empty lists
- [ ] `insertAtTail` handles empty list case
- [ ] `insertAtIndex` handles index 0, index == size, and middle cases
- [ ] `removeAtHead` handles empty list, single element, and normal cases
- [ ] `removeAtIndex` handles index 0 and middle cases
- [ ] `getElement` validates bounds, handles NULL
- [ ] `getSize` handles NULL input
- [ ] All functions handle NULL list input
- [ ] No memory leaks (verified with Valgrind)
- [ ] Code compiles without warnings

---

## Summary

This lab teaches fundamental linked list operations:

| Concept | How It Appears |
|---------|----------------|
| **Node-based structure** | Each element is a separate allocation |
| **Pointer chasing** | Traversing via `current = current->next` |
| **Pointer rewiring** | Changing `next` pointers to insert/remove |
| **Edge cases** | Empty lists, single elements, boundaries |
| **Memory management** | Allocating nodes, freeing in correct order |

Linked lists are foundational to more complex data structures like trees, graphs, and hash tables. Master these operations now—you'll use them throughout your career!

Good luck!
