#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lab4.h"

extern Node *g_root;

/* ----------------------------------------------------------------
 * TODO 29  check_integrity
 *
 * Use BFS to verify:
 *   - Every question node has both yes and no children (non-NULL).
 *   - Every solution node has both children NULL.
 * Return 1 if valid, 0 if any violation is found.
 * ---------------------------------------------------------------- */
int check_integrity(void) {
    if (g_root == NULL) return 0;


    // BFS
    Queue q;
    q_init(&q);
    q_enqueue(&q,g_root, 0);
    
    int next_id = 1;
    int index = 0;
    while(!q_empty(&q)){

        Node *CurrentNode = NULL;
        int id;
        q_dequeue(&q, &CurrentNode,&id);
        index++;

        if(CurrentNode->isQuestion == 1 && (CurrentNode->yes == NULL || CurrentNode->no == NULL)){
            q_free(&q);
            return 0;
        }
        else if(CurrentNode->isQuestion == 0 && (CurrentNode->yes != NULL || CurrentNode->no != NULL)){
            q_free(&q);
            return 0;
        }
        if (CurrentNode->yes != NULL){
            q_enqueue(&q,CurrentNode->yes,next_id);
            next_id++;
        }
        if (CurrentNode->no != NULL){
            q_enqueue(&q,CurrentNode->no,next_id);
            next_id++;
        }
    }
    q_free(&q);
    return 1;
}

/* ----------------------------------------------------------------
 * TODO 30  find_shortest_path
 *
 * Given the exact text of two solution leaves, display the
 * questions that distinguish them.  Use BFS with a parent-tracking
 * PathNode array to find both leaves, build ancestor arrays for
 * each, find the Lowest Common Ancestor (LCA), then print:
 *   - The shared path of questions both solutions pass through.
 *   - The divergence question (LCA) and which branch leads where.
 *
 * Display results with mvprintw.  Print an error if either
 * solution is not found.  Free all allocations before returning.
 * ---------------------------------------------------------------- */
void find_shortest_path(const char *sol1, const char *sol2) {
    if (g_root == NULL) {
        mvprintw(10, 2, "Error: knowledge base is empty.");
        refresh();
        return;
    }
    mvprintw(10, 2, "find_shortest_path not yet implemented.");
    refresh();
}
