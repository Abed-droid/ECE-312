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
    while(!q_empty(&q)){

        Node *CurrentNode = NULL;
        int id;
        q_dequeue(&q, &CurrentNode,&id);

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
typedef struct {
    Node *node;        // The actual tree node
    int parent_id;     // The array index of this node's parent
    int branch_taken;  // 1 if it was a 'yes' child, 0 if 'no'
} PathNode;

void find_shortest_path(const char *sol1, const char *sol2) {
    if (g_root == NULL) {
        mvprintw(10, 2, "Error: knowledge base is empty.");
        refresh();
        return;
    }
    if (check_integrity() != 1) return;
    // pathNode array
    int nodeCount = count_nodes(g_root);
    PathNode *pathNode = malloc(sizeof(PathNode) * nodeCount);
    if (pathNode == NULL) return;
    // BFS
    Queue q;
    q_init(&q);
    pathNode[0] = (PathNode){ g_root, -1, -1 };
    q_enqueue(&q,g_root,0);
    int nextid = 1;

    int sol1Index = -1;
    int sol2Index = -1;
    while(!q_empty(&q)){
        Node* CurrentNode = NULL;
        int id;
        q_dequeue(&q,&CurrentNode , &id);

        // search if currentNode->text matches a solution
        if (strcmp(CurrentNode->text,sol1 ) == 0){
            sol1Index = id;
        }
        if (strcmp(CurrentNode->text,sol2 ) == 0 ){
            sol2Index = id;
        }
        // if we found both solution indexes we break out while loop
        if (sol2Index != -1 && sol1Index !=-1) break;

        if (CurrentNode->yes != NULL){
            pathNode[nextid] = (PathNode){CurrentNode->yes, id, 1};
            q_enqueue(&q,CurrentNode->yes,nextid);
            nextid++;
        }
        if (CurrentNode->no != NULL){
            pathNode[nextid] = (PathNode){CurrentNode->no, id, 0};
            q_enqueue(&q,CurrentNode->no,nextid);
            nextid++;
        }
    }
    int *ancestors1IDs = malloc(sizeof(int)* nodeCount);
    int *ancestors2IDs = malloc(sizeof(int) * nodeCount);
    if (ancestors1IDs == NULL || ancestors2IDs == NULL) {
        free(ancestors1IDs);
        free(ancestors2IDs);
        free(pathNode);
        q_free(&q);
        return;
    }

    int row = 8;

    if (sol1Index == -1 || sol2Index == -1) {
        mvprintw(row, 2, "Error: one or both solutions not found.");
        refresh();
        q_free(&q);
        free(ancestors1IDs);
        free(ancestors2IDs);
        free(pathNode);
        return;
    }

    int len1 = 0;
    int cur = sol1Index;
    while (cur != -1) {
        ancestors1IDs[len1] = cur;
        len1++;
        cur = pathNode[cur].parent_id;
    }
    cur = sol2Index;
    int len2 = 0;
    while (cur != -1) {
        ancestors2IDs[len2] = cur;
        len2++;
        cur = pathNode[cur].parent_id;
    }

    // Reverse both arrays to root-to-leaf order
    for (int left = 0, right = len1 - 1; left < right; left++, right--) {
        int tmp = ancestors1IDs[left];
        ancestors1IDs[left] = ancestors1IDs[right];
        ancestors1IDs[right] = tmp;
    }
    for (int left = 0, right = len2 - 1; left < right; left++, right--) {
        int tmp = ancestors2IDs[left];
        ancestors2IDs[left] = ancestors2IDs[right];
        ancestors2IDs[right] = tmp;
    }

    // Walk forward until paths diverge to find LCA
    int lca = 0;
    while (lca < len1 && lca < len2 && ancestors1IDs[lca] == ancestors2IDs[lca]) {
        lca++;
    }
    lca--;

    mvprintw(row++, 2, "Shared path:");
    for (int k = 0; k < lca; k++) {
        int idx = ancestors1IDs[k];
        int child_idx = ancestors1IDs[k + 1];
        if (pathNode[child_idx].branch_taken == 1) {
            mvprintw(row++, 4, "Q: %s -> YES", pathNode[idx].node->text);
        } else {
            mvprintw(row++, 4, "Q: %s -> NO", pathNode[idx].node->text);
        }
    }

    mvprintw(row++, 2, "Divergence: %s", pathNode[ancestors1IDs[lca]].node->text);
    int sol1Branch = pathNode[ancestors1IDs[lca + 1]].branch_taken;
    if (sol1Branch == 1) {
        mvprintw(row++, 4, "YES -> %s", sol1);
        mvprintw(row++, 4, "NO  -> %s", sol2);
    } else {
        mvprintw(row++, 4, "YES -> %s", sol2);
        mvprintw(row++, 4, "NO  -> %s", sol1);
    }
    refresh();

    q_free(&q);
    free(ancestors1IDs);
    free(ancestors2IDs);
    free(pathNode);


    


    
    

}
