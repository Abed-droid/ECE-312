# ECE 312: Lab 3 Memory Pool Allocator

**Course:** ECE 312 – Software Design and Implementation I  
**Topic:** Dynamic Memory Management, Free Lists, Memory Fragmentation
**Due:** Monday, 3/2/2026 @ 11:59pm 

---

## Overview

In this lab, you will implement a **memory pool allocator** — a custom memory manager that operates on a fixed-size block of memory. This is similar to what happens "under the hood" when you call `malloc()` and `free()`, but simplified for educational purposes.

Memory pool allocators are used in real systems (game engines, embedded systems, network servers) to provide more predictable performance and avoid fragmentation issues that can occur with general-purpose allocators.

---

## Learning Objectives

By completing this lab, you will:

- Understand how memory allocators manage free and allocated regions
- Implement a **doubly-linked free list** to track available memory
- Apply **first-fit allocation** to find suitable blocks for requests
- Implement **block splitting** to reduce wasted space
- Implement **coalescing** to merge adjacent free blocks
- Handle edge cases like double-free, invalid pointers, and oversized requests
- Calculate and understand **memory fragmentation**

---

## Background

### How malloc() Works (Simplified)

When you call `malloc(100)`, the system doesn't just magically find 100 bytes. There's a memory manager that:

1. Maintains a pool of available memory
2. Tracks which regions are free and which are allocated
3. Finds a free region large enough for your request
4. Marks that region as allocated and returns a pointer

When you call `free(ptr)`, the manager marks that region as free again.

### The Memory Pool Model

Your allocator will manage a **4096-byte pool** with these components:

```
┌────────────────────────────────────────────────────────────────┐
│                     Memory Pool (4096 bytes)                   │
├─────────────┬─────────────┬─────────────┬─────────────┬────────┤
│  Block A    │  Block B    │  Block C    │  Block D    │  ...   │
│  (alloc)    │  (free)     │  (alloc)    │  (free)     │        │
└─────────────┴─────────────┴─────────────┴─────────────┴────────┘
```

Each block has a **header** that tracks:
- `size` — payload size in bytes
- `is_free` — whether the block is available
- `data` — pointer to the actual memory in the pool
- `prev/next` — links for the free list (doubly-linked list)
- `all_prev/all_next` — links for the master list of all blocks

---

### ⚠️ CRITICAL: Where Headers Live vs. Where Payloads Live

This is one of the most important things to understand in this lab:

**Block headers (`MemoryBlock` structs) are allocated on the HEAP using `malloc()`. They do NOT live inside the 4096-byte pool and do NOT consume any of the pool's memory.**

**Only payloads (the actual data the user requested) live inside the 4096-byte pool.**

Think of it this way: the headers are "bookkeeping" metadata that sits *outside* the pool, while the pool itself is the raw memory being managed.

```
                HEAP (separate memory)               THE POOL (4096 bytes)
        ┌──────────────────────────┐          ┌──────────────────────────────┐
        │  MemoryBlock header A    │          │                              │
        │    size = 104            │  .data ──┼──►[ 104 bytes of payload A ] │
        │    is_free = 0           │          │                              │
        │    next ──┐              │          ├──────────────────────────────┤
        └───────────┼──────────────┘          │                              │
        ┌───────────▼──────────────┐          │                              │
        │  MemoryBlock header B    │  .data ──┼──►[ 200 bytes of payload B ] │
        │    size = 200            │          │                              │
        │    is_free = 1           │          ├──────────────────────────────┤
        │    next ──┐              │          │                              │
        └───────────┼──────────────┘          │  ... remaining pool space ...│
        ┌───────────▼──────────────┐          │                              │
        │  MemoryBlock header C    │  .data ──┼──► [ 3792 bytes payload C ]  │
        │    size = 3792           │          │                              │
        │    is_free = 1           │          │                              │
        └──────────────────────────┘          └──────────────────────────────┘
```

This means:
- When you call `pool_init()`, the **entire** 4096 bytes of the pool is available as free memory (`free_size = 4096`). The initial `MemoryBlock` header is separately `malloc()`'d.
- When you call `split_block()`, you `malloc()` a **new** `MemoryBlock` header on the heap — you do NOT carve space for it out of the pool.
- The sum of all block sizes (free + allocated) should always equal 4096.

**Why this matters:** If headers lived inside the pool, you would need to subtract `sizeof(MemoryBlock)` from the available space for each block. In our design, headers are external, so the pool's full 4096 bytes are available for payloads.

---

### The Doubly-Linked Free List (Your Implementation Focus)

The core data structure you will implement is a **doubly-linked list** of free blocks, sorted by the address of their payload in the pool. This is the `free_list` in the `MemoryPool` struct. You are responsible for implementing the insertion, removal, and traversal of this doubly-linked list through these functions:

- **`add_to_free_list()`** — Insert a node into the doubly-linked list in sorted (address) order
- **`remove_from_free_list()`** — Remove a node from the doubly-linked list by updating `prev`/`next` pointers
- **`find_free_block()`** — Traverse the doubly-linked list to find a suitable block (first-fit)
- **`coalesce_blocks()`** — Walk the doubly-linked list and merge adjacent free nodes

Each `MemoryBlock` has `prev` and `next` pointers that form the doubly-linked free list:

```
g_pool.free_list
       │
       ▼
   ┌────────┐     ┌────────┐     ┌────────┐
   │Block B │◄───►│Block D │◄───►│Block F │───► NULL
   │addr 104│     │addr 408│     │addr 808│
   │size 200│     │size 96 │     │size 328│
   └────────┘     └────────┘     └────────┘
       ▲
       │
      NULL (B->prev is NULL because B is the head)
```

The free list is **address-ordered** — blocks are sorted by their `data` pointer values. This ordering is critical because it makes coalescing efficient: if two consecutive nodes in the free list point to adjacent memory regions in the pool, they can be merged.

**There is also a separate doubly-linked "master" list (`all_list`)** that tracks ALL blocks (free and allocated). The helper functions for `all_list` are provided for you — you do NOT need to implement those. Your focus is on the `free_list` doubly-linked list operations.

#### Quick Reference: Which List Does What?

| List | Pointers Used | Contains | You Implement? |
|------|--------------|----------|----------------|
| `free_list` | `prev`, `next` | Only free blocks (address-sorted) | **YES** |
| `all_list` | `all_prev`, `all_next` | All blocks (free + allocated) | No (provided) |

---

### Block Splitting

When you allocate less than a free block's size, split it:

```
Before: [────────── FREE (500 bytes) ──────────]

pool_malloc(100)   (assume 100 is already aligned)

After:  [─ ALLOC (100) ─][─── FREE (400) ───]
```

The new free block's header is `malloc()`'d separately on the heap. Only the split happens inside the pool — the first 100 bytes become the allocated payload, the remaining 400 bytes become a new free block's payload.

**Worked Example — Splitting Step by Step:**

Starting state: One free block, size = 4096, data points to start of pool.

1. `pool_malloc(100)` is called. After alignment, size = 104 (since `ALIGN(100)` rounds up to the next multiple of 8).
2. `find_free_block(104)` finds the one free block (size 4096 ≥ 104).
3. The block is removed from the free list.
4. `split_block(block, 104)` is called:
   - Leftover = 4096 - 104 = 3992 bytes. Since 3992 ≥ ALIGNMENT (8), we split.
   - A new `MemoryBlock` header is `malloc()`'d on the heap.
   - New block: size = 3992, data = pool_start + 104, is_free = 1.
   - Original block: size shrinks to 104.
   - New block is inserted into both `all_list` and `free_list`.
5. The original block is marked `is_free = 0`.
6. `free_size` decreases by 104 (from 4096 to 3992).
7. `pool_malloc` returns `block->data` (pointer to the first 104 bytes of the pool).

---

### Coalescing

When you free a block adjacent to another free block, merge them:

```
Before: [FREE A (104)][ALLOC B (104)][FREE C (104)]

pool_free(B)

Step 1 - B is added to free list:
        [FREE A (104)][FREE B (104)][FREE C (104)]

Step 2 - coalesce_blocks() merges adjacent:
        [──────────── FREE (312 bytes) ────────────]
```

Without coalescing, the pool becomes fragmented — lots of small free blocks that can't satisfy large requests.

**Worked Example — Coalescing Step by Step:**

Suppose the free list (in address order) looks like:

```
Free list: [Block at addr 0, size 104] <-> [Block at addr 104, size 104] <-> [Block at addr 304, size 3792]
```

1. `coalesce_blocks()` starts at the head of the free list.
2. Check: does `0 + 104 == 104`? YES — blocks are adjacent. Merge:
   - First block absorbs second: size becomes 104 + 104 = 208.
   - Second block's header is removed from free_list, all_list, and `free()`'d.
   - `total_blocks` decreases by 1.
   - Stay on the first block (don't advance) to check for further merges.
3. Check: does `0 + 208 == 304`? NO (208 ≠ 304) — blocks are not adjacent. Advance.
4. End of free list — done.

Result: `[Block at addr 0, size 208] <-> [Block at addr 304, size 3792]`

---

### Alignment

All allocation sizes are rounded up to the nearest multiple of 8 using the `ALIGN()` macro:

```c
#define ALIGNMENT 8
#define ALIGN(sz) (((sz) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

ALIGN(1)   = 8
ALIGN(7)   = 8
ALIGN(8)   = 8
ALIGN(9)   = 16
ALIGN(100) = 104
ALIGN(256) = 256
```

This ensures all payload regions start and end on 8-byte boundaries, which is important for hardware performance and correctness on most architectures.

---

## Assignment

### Files Provided

| File | Description |
|------|-------------|
| `mempool.h` | Header file with structures and function declarations |
| `mempool.c` | Starter file — implement your functions here |
| `mempool_test.c` | Test harness that reads script files |
| `test1.txt` | Basic allocation and deallocation |
| `test2.txt` | Fragmentation and coalescing |
| `test3.txt` | Edge cases (zero-size, double-free, invalid pointer) |
| `test4.txt` | Block splitting and size accounting |
| `test5.txt` | Complex alloc/free interleaving patterns |
| `test6.txt` | Fill the pool, free everything, verify recovery |
| `test7.txt` | Coalescing chains of 3+ adjacent blocks |

### Functions to Implement

#### 1. `pool_init`

```c
int pool_init(void);
```

Initializes the memory pool.

**Requirements:**
- Check if pool is already initialized (return -1 if so)
- Allocate POOL_SIZE (4096) bytes using `malloc()` — this is the pool
- Create one initial free block header using `malloc()` — this header lives **on the heap, not in the pool**
- The initial block's `size` should be `POOL_SIZE` (the full pool is available)
- Initialize all `g_pool` fields
- Return 0 on success, -1 on failure

**Example state after `pool_init()`:**
```
g_pool.pool_start  → [──────── 4096 bytes of pool memory ────────]
g_pool.total_size  = 4096
g_pool.free_size   = 4096     ← ALL 4096 bytes are free (headers don't count)
g_pool.total_blocks = 1
g_pool.free_blocks  = 1
g_pool.free_list   → [MemoryBlock: size=4096, is_free=1, data=pool_start]
g_pool.all_list    → [same MemoryBlock]
```

---

#### 2. `pool_cleanup`

```c
void pool_cleanup(void);
```

Frees all memory associated with the pool.

**Requirements:**
- Return early if `pool_start` is NULL
- Free all block headers by traversing `all_list` (save `all_next` before freeing!)
- Free the pool memory itself (`pool_start`)
- Zero out `g_pool` using `memset`

---

#### 3. `find_free_block`

```c
MemoryBlock* find_free_block(size_t size);
```

Finds a free block of at least `size` bytes using **first-fit**.

**Requirements:**
- Traverse `free_list` from head
- Return first block where `block->size >= requested size`
- Return NULL if no suitable block exists
- Do NOT modify the free list — just search it

**Example:**
```
Free list: [size=104, addr=200] <-> [size=400, addr=500] <-> [size=50, addr=1000]

find_free_block(80)   → returns block at addr 200 (first one ≥ 80)
find_free_block(200)  → returns block at addr 500 (first one ≥ 200)
find_free_block(500)  → returns NULL (nothing big enough)
```

---

#### 4. `add_to_free_list`

```c
void add_to_free_list(MemoryBlock* block);
```

Inserts a block into the **doubly-linked** free list in **address order**.

**This is a doubly-linked list insertion.** You must update both `prev` and `next` pointers.

**Requirements:**
- Set `block->is_free = 1`
- Increment `g_pool.free_blocks`
- Walk the free list to find the correct position based on `block->data` address
- Insert the block, updating `prev`/`next` pointers of neighbors
- Handle all cases: empty list, insert at head, insert in middle, insert at tail

**Example — Inserting into the middle:**
```
Before: [addr=100] <-> [addr=500]

add_to_free_list(block with addr=300)

After:  [addr=100] <-> [addr=300] <-> [addr=500]
```

**Example — Inserting at the head:**
```
Before: [addr=300] <-> [addr=500]

add_to_free_list(block with addr=100)

After:  [addr=100] <-> [addr=300] <-> [addr=500]
        ▲
        g_pool.free_list now points here
```

---

#### 5. `remove_from_free_list`

```c
void remove_from_free_list(MemoryBlock* block);
```

Removes a block from the **doubly-linked** free list.

**This is a doubly-linked list removal.** Update the neighbors to skip this node.

**Requirements:**
- If block has a `prev`, set `prev->next = block->next`
- If block is the head, update `g_pool.free_list = block->next`
- If block has a `next`, set `next->prev = block->prev`
- Clear `block->prev` and `block->next`
- Decrement `g_pool.free_blocks`
- Do NOT modify `is_free` — that's done by the caller

---

#### 6. `split_block`

```c
void split_block(MemoryBlock* block, size_t size);
```

Splits a block if there's enough leftover space.

**Requirements:**
- Only split if `block->size >= size + ALIGNMENT` (minimum for a useful remainder)
- Calculate `leftover = block->size - size`
- Allocate a NEW `MemoryBlock` header using `malloc()` — **on the heap, NOT from the pool**
- Set up the new block: `size = leftover`, `is_free = 1`, `data = (char*)block->data + size`
- Add new block to `all_list` using `all_list_insert_after(block, newblock)`
- Add new block to `free_list` using `add_to_free_list(newblock)`
- Update original block's `size = size`
- Increment `g_pool.total_blocks`

**Example:**
```
Before split_block(block, 104):
  block: size=4096, data=pool_start

After:
  block:    size=104,  data=pool_start          (will be marked allocated by caller)
  newblock: size=3992, data=pool_start + 104    (free, in both lists)
```

---

#### 7. `coalesce_blocks`

```c
void coalesce_blocks(void);
```

Merges adjacent free blocks in the **doubly-linked** free list.

**Requirements:**
- Walk `free_list` checking each consecutive pair
- Two blocks are physically adjacent if: `(char*)cur->data + cur->size == (char*)nxt->data`
- When merging: absorb `nxt` into `cur`, remove `nxt` from free_list and all_list, `free(nxt)`
- Decrement `total_blocks` for each merge
- **Stay on `cur` after merge** — don't advance! There might be a third adjacent block.

**Example — Chain of 3:**
```
Free list: [addr=0, size=104] <-> [addr=104, size=104] <-> [addr=208, size=104]

Iteration 1: 0+104 == 104? YES → merge. cur becomes [addr=0, size=208]
Iteration 2: 0+208 == 208? YES → merge. cur becomes [addr=0, size=312]
Iteration 3: cur->next is NULL → done.

Result: [addr=0, size=312]
```

---

#### 8. `pool_malloc`

```c
void* pool_malloc(size_t size);
```

Allocates memory from the pool.

**Requirements:**
- Return NULL if `size == 0`
- Align size using `ALIGN(size)`
- Return NULL if aligned size > `free_size` (quick reject)
- Find a block using `find_free_block()`
- Remove block from free list
- Split if there's leftover space
- Mark as allocated (`is_free = 0`), subtract size from `free_size`
- Return `block->data`

**Walkthrough — Full `pool_malloc(100)` call:**
```
1. size = ALIGN(100) = 104              (round up to 8-byte boundary)
2. 104 ≤ free_size (4096)?  YES         (proceed)
3. find_free_block(104) → block         (found a block with size ≥ 104)
4. remove_from_free_list(block)         (take it off the free list)
5. split_block(block, 104)              (split if block.size ≥ 104 + 8)
6. block->is_free = 0                   (mark allocated)
7. g_pool.free_size -= 104              (update accounting: 4096 → 3992)
8. return block->data                   (pointer into the pool)
```

---

#### 9. `pool_free`

```c
void pool_free(void* ptr);
```

Frees a previously allocated block.

**Requirements:**
- Return if `ptr` is NULL
- Validate pointer is in pool using `ptr_in_pool()`
- Find the block header using `find_block_by_ptr()`
- Check for double-free (`is_free` already 1) — print error and return
- Add `block->size` back to `free_size`
- Add block to free list using `add_to_free_list()`
- Coalesce with neighbors using `coalesce_blocks()`

---

## Test Script Commands

The test harness reads commands from text files:

| Command | Description |
|---------|-------------|
| `INIT` | Initialize the pool |
| `MALLOC id size` | Allocate `size` bytes, store as `id` |
| `FREE id` | Free the allocation named `id` |
| `STATS` | Print pool statistics |
| `COALESCE` | Explicitly coalesce free blocks |
| `CLEANUP` | Free all pool memory |
| `QUIT` | Exit |
| `# comment` | Ignored |

**Example script with expected reasoning:**
```
INIT                    # Pool: 4096 bytes free, 1 block
MALLOC a 100            # Allocates 104 bytes (aligned). Free: 3992. Blocks: 2 (1 alloc + 1 free)
MALLOC b 200            # Allocates 200 bytes (already aligned). Free: 3792. Blocks: 3
STATS                   # Should show: free=3792, total_blocks=3, free_blocks=1
FREE a                  # Returns 104 bytes. Free: 3896. free_blocks=2 (but coalescing won't help — b is between them)
FREE b                  # Returns 200 bytes. Free: 4096. Coalescing merges all 3 free blocks into 1
STATS                   # Should show: free=4096, total_blocks=1, free_blocks=1, fragmentation=0%
CLEANUP
QUIT
```

---

## Compiling and Testing

### Compile

```bash
gcc -Wall -Wextra -o mempool_test mempool.c mempool_test.c
```

### Run with a test script

```bash
./mempool_test
Enter the name of the commands file: test1.txt
```

### Check for memory leaks

```bash
valgrind --leak-check=full ./mempool_test
```

### Run all test scripts at once

```bash
for f in test*.txt; do echo "=== $f ==="; echo "$f" | ./mempool_test; echo; done
```

---

## Common Mistakes

### 1. Thinking headers live in the pool

```c
// WRONG — subtracting header size from pool
block->size = POOL_SIZE - sizeof(MemoryBlock);

// CORRECT — headers are on the heap, the full pool is available
block->size = POOL_SIZE;
```

The `MemoryBlock` structs are allocated with `malloc()` and live on the heap. The pool's 4096 bytes are entirely for payloads.

### 2. Forgetting address ordering in free list

```c
// WRONG - inserting at head always
block->next = g_pool.free_list;
g_pool.free_list = block;

// CORRECT - find position based on address
MemoryBlock* cur = g_pool.free_list;
MemoryBlock* prev = NULL;
while (cur && cur->data < block->data) {
    prev = cur;
    cur = cur->next;
}
// Insert between prev and cur, update both prev and next pointers
```

### 3. Not updating both `prev` AND `next` in doubly-linked list operations

```c
// WRONG — only updating next pointers (this is a singly-linked list approach!)
prev->next = block;
block->next = cur;

// CORRECT — update all four connections for a doubly-linked list
block->prev = prev;
block->next = cur;
if (prev) prev->next = block;
else g_pool.free_list = block;
if (cur) cur->prev = block;
```

### 4. Not coalescing after free

Freeing a block without coalescing leads to fragmentation. Always call `coalesce_blocks()` at the end of `pool_free()`.

### 5. Freeing headers before unlinking

```c
// WRONG - lose access to list
free(node);
node->next->prev = node->prev;  // CRASH! node is already freed

// CORRECT - unlink first, then free
remove_from_free_list(node);
all_list_remove(node);
free(node);
```

### 6. Off-by-one in adjacency check

```c
// Blocks are adjacent if:
(char*)cur->data + cur->size == (char*)nxt->data
// NOT:
(char*)cur->data + cur->size + 1 == (char*)nxt->data  // WRONG
```

### 7. Advancing the pointer after a merge in coalesce

```c
// WRONG — skips potential chain merges
if (adjacent) {
    merge(cur, nxt);
    cur = cur->next;  // BUG: might miss another adjacent block
}

// CORRECT — stay on cur after merge
if (adjacent) {
    merge(cur, nxt);
    // do NOT advance cur — check cur against new cur->next
} else {
    cur = cur->next;
}
```

### 8. Not handling empty free list

```c
// Always check before dereferencing
if (!g_pool.free_list) {
    block->prev = block->next = NULL;
    g_pool.free_list = block;
    return;
}
```

---

## Understanding Fragmentation

**Fragmentation** occurs when free memory is scattered in small chunks:

```
Pool: [ALLOC][free][ALLOC][free][ALLOC][free][ALLOC]
       100    50    100    50    100    50    100

Total free: 150 bytes
Largest free block: 50 bytes
Can't allocate 100 bytes even though 150 are free!
```

The `pool_stats()` function calculates fragmentation as:

```
fragmentation = 1 - (largest_free_block / total_free_memory)
```

- 0% = all free memory in one block (ideal)
- High % = memory scattered in small pieces (bad)

---

## Submission Instructions

1. Complete all functions in `mempool.c`
2. Test with **all** provided scripts (test1.txt through test7.txt) and write your own
3. Verify no memory leaks with Valgrind
4. Submit `mempool.c` to Gradescope
5. Review feedback and iterate

**Reminder:** Hidden test cases will check edge cases you might not have considered. Test thoroughly!

---

## Checklist

- [ ] `pool_init` allocates pool and creates initial free block (header on heap, full pool available)
- [ ] `pool_cleanup` frees all headers and pool memory
- [ ] `find_free_block` implements first-fit correctly
- [ ] `add_to_free_list` maintains address order with correct doubly-linked list insertion
- [ ] `remove_from_free_list` properly unlinks blocks (updates both `prev` and `next`)
- [ ] `split_block` creates valid remainder blocks (header `malloc()`'d on heap, not from pool)
- [ ] `coalesce_blocks` merges all adjacent free blocks (handles chains of 3+)
- [ ] `pool_malloc` aligns sizes, splits blocks, updates stats
- [ ] `pool_free` validates pointers, detects double-free, coalesces
- [ ] No memory leaks (all headers and pool freed in cleanup)
- [ ] Edge cases handled (NULL, zero size, oversized requests)
- [ ] Sum of all block sizes always equals POOL_SIZE (4096)

---

## Summary

This lab teaches core memory management concepts:

| Concept | Implementation |
|---------|----------------|
| **Free list** | Doubly-linked, address-ordered list of free blocks |
| **First-fit** | Find first block large enough for request |
| **Splitting** | Divide large blocks to reduce waste (new header on heap) |
| **Coalescing** | Merge adjacent free blocks to reduce fragmentation |
| **Fragmentation** | Measure of how scattered free memory is |

These concepts appear in operating systems, runtime libraries, and any system that manages memory. Understanding them will make you a better systems programmer!

Good luck!
