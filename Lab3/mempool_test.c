/*
 * ECE 312 - Memory Pool Allocator Lab
 * Test Harness (DO NOT MODIFY)
 * 
 * This program reads commands from a script file and executes them
 * against your memory pool implementation.
 * 
 * Usage: ./mempool_test
 *        (then enter the script filename when prompted)
 * 
 * Commands:
 *   INIT              - Initialize the memory pool
 *   MALLOC id size    - Allocate 'size' bytes, store pointer as 'id'
 *   FREE id           - Free the allocation with identifier 'id'
 *   STATS             - Print pool statistics
 *   COALESCE          - Explicitly coalesce free blocks
 *   CLEANUP           - Free all pool memory
 *   QUIT              - Exit the program
 *   # comment         - Lines starting with # are ignored
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mempool.h"

#define MAX_IDS 1024

typedef struct {
    char name[32];
    void* ptr;
} Slot;

static Slot g_slots[MAX_IDS];

static int find_slot(const char* name) {
    for (int i = 0; i < MAX_IDS; ++i)
        if (g_slots[i].name[0] && strcmp(g_slots[i].name, name) == 0) return i;
    return -1;
}

static int ensure_slot(const char* name) {
    int idx = find_slot(name);
    if (idx >= 0) return idx;
    for (int i = 0; i < MAX_IDS; ++i)
        if (!g_slots[i].name[0]) { 
            strncpy(g_slots[i].name, name, sizeof(g_slots[i].name)-1); 
            return i; 
        }
    return -1;
}

static void trim_newline(char* s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = 0;
}

static void run_script(FILE* fp) {
    char line[256];
    int line_no = 0;
    
    while (fgets(line, sizeof line, fp)) {
        line_no++;
        trim_newline(line);
        
        char* p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == 0 || *p == '#') continue;

        char cmd[32], a[64];
        size_t size = 0;
        a[0] = 0;
        
        if (sscanf(p, "%31s %63s %zu", cmd, a, &size) < 1) continue;

        if (strcmp(cmd, "INIT") == 0) {
            if (pool_init() != 0) {
                fprintf(stderr, "INIT failed\n");
                return;
            }
            printf("INIT\n");
        } else if (strcmp(cmd, "MALLOC") == 0) {
            if (!a[0] || size == 0) {
                fprintf(stderr, "Line %d: MALLOC <id> <size>\n", line_no);
                continue;
            }
            int idx = ensure_slot(a);
            if (idx < 0) {
                fprintf(stderr, "Line %d: ID table full\n", line_no);
                continue;
            }
            g_slots[idx].ptr = pool_malloc(size);
            printf("MALLOC %s %zu -> %s\n", a, size, 
                   g_slots[idx].ptr ? "OK" : "FAILED");
        } else if (strcmp(cmd, "FREE") == 0) {
            if (!a[0]) {
                fprintf(stderr, "Line %d: FREE <id>\n", line_no);
                continue;
            }
            int idx = find_slot(a);
            if (idx < 0 || !g_slots[idx].ptr) {
                fprintf(stderr, "Line %d: unknown id '%s'\n", line_no, a);
                continue;
            }
            printf("FREE %s\n", a);
            pool_free(g_slots[idx].ptr);
            g_slots[idx].ptr = NULL;
        } else if (strcmp(cmd, "STATS") == 0) {
            pool_stats();
        } else if (strcmp(cmd, "COALESCE") == 0) {
            coalesce_blocks();
            printf("COALESCE\n");
        } else if (strcmp(cmd, "CLEANUP") == 0) {
            pool_cleanup();
            printf("CLEANUP\n");
        } else if (strcmp(cmd, "QUIT") == 0) {
            printf("QUIT\n");
            break;
        } else {
            fprintf(stderr, "Line %d: unknown command '%s'\n", line_no, cmd);
        }
    }
}

int main(int argc, char** argv) {
    char filename[256];
    
    printf("Enter the name of the commands file: ");
    fflush(stdout);
    
    if (!fgets(filename, sizeof(filename), stdin)) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    printf("\n");
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "No filename provided\n");
        return 1;
    }
    
    FILE* fp = fopen(filename, "r");
    if (!fp) { 
        perror(filename); 
        return 1; 
    }
    
    run_script(fp);
    fclose(fp);
    
    if (g_pool.pool_start) pool_cleanup();
    return 0;
}
