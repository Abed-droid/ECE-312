#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stddef.h>

/*
 * Memory Pool Allocator
 * 
 * This module implements a simple memory pool allocator that manages
 * a fixed-size block of memory (4096 bytes). The allocator supports:
 *   - First-fit allocation strategy
 *   - Block splitting when allocating less than available
 *   - Coalescing of adjacent free blocks
 *   - Address-ordered free list
 */

#define POOL_SIZE 4096
#define ALIGNMENT 8
#define ALIGN(sz) (((sz) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/*
 * MemoryBlock: Header for each block in the pool
 * 
 * Each block (free or allocated) has a header containing metadata.
 * The 'data' field points to the payload region in the pool.
 */
typedef struct MemoryBlock {
    size_t size;                 // Payload size in bytes (aligned, excludes header)
    int    is_free;              // 1 = free, 0 = allocated
    void*  data;                 // Pointer to payload in the pool
    
    struct MemoryBlock* next;    // Next block in free list (address order)
    struct MemoryBlock* prev;    // Previous block in free list
    
    struct MemoryBlock* all_next; // Next block in master list (all blocks)
    struct MemoryBlock* all_prev; // Previous block in master list
} MemoryBlock;

/*
 * MemoryPool: Global state for the allocator
 */
typedef struct {
    void*  pool_start;           // Base address of the 4096-byte pool
    size_t total_size;           // Always POOL_SIZE (4096)
    size_t free_size;            // Sum of sizes of all FREE payloads
    MemoryBlock* free_list;      // Head of free blocks (address-sorted)
    MemoryBlock* all_list;       // Head of all blocks (for lookup)
    int    total_blocks;         // Count of all blocks (free + allocated)
    int    free_blocks;          // Count of free blocks only
} MemoryPool;

/* Global pool instance (defined in mempool.c) */
extern MemoryPool g_pool;

/*
 * Initialize the memory pool.
 * Allocates POOL_SIZE bytes and creates one large free block.
 * Returns 0 on success, -1 on failure.
 */
int pool_init(void);

/*
 * Clean up the memory pool.
 * Frees all block headers and the pool memory itself.
 */
void pool_cleanup(void);

/*
 * Find a free block of at least 'size' bytes using first-fit.
 * Returns pointer to the block header, or NULL if none found.
 * Does NOT remove the block from the free list.
 */
MemoryBlock* find_free_block(size_t size);

/*
 * Split a block if it's large enough to create a meaningful remainder.
 * After splitting, the original block has size 'size' and a new free
 * block is created for the leftover space.
 */
void split_block(MemoryBlock* block, size_t size);

/*
 * Remove a block from the free list.
 * Updates prev/next pointers and decrements free_blocks count.
 * Does NOT change is_free flag.
 */
void remove_from_free_list(MemoryBlock* block);

/*
 * Add a block to the free list in address order.
 * Sets is_free = 1 and increments free_blocks count.
 */
void add_to_free_list(MemoryBlock* block);

/*
 * Merge adjacent free blocks in the free list.
 * Two blocks are adjacent if one's payload ends where the next begins.
 */
void coalesce_blocks(void);

/*
 * Allocate 'size' bytes from the pool.
 * Returns pointer to usable memory, or NULL if allocation fails.
 */
void* pool_malloc(size_t size);

/*
 * Free a previously allocated block.
 * Adds it back to the free list and coalesces with neighbors.
 */
void pool_free(void* ptr);

/*
 * Print statistics about the memory pool.
 * Shows total size, free memory, block counts, and fragmentation.
 */
void pool_stats(void);

#endif
