#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lab4.h"

/* ----------------------------------------------------------------
 * ds.c  --  all data structures for the Tech Support Diagnosis Tool
 *
 * Implement every function marked TODO.  The only functions in this
 * entire lab permitted to use recursion are free_tree and count_nodes.
 * Everything else must be iterative.
 * ---------------------------------------------------------------- */


/* ====== Tree nodes ============================================== */

/* TODO 1 */
Node *create_question_node(const char *question) {
    if (question == NULL) return NULL;
    // Create Node pointer
    Node* question_node = (Node*) malloc(sizeof(Node));
    if (question_node == NULL) return NULL;
    question_node->isQuestion = 1;
    // First allocate memory for question on heap then assign it to question_node->text
    // same as strdup(question_node->text, question);
    question_node->text = (char*) malloc(sizeof(char) * (strlen(question) + 1));
    if (question_node->text == NULL){
        free(question_node);
        return NULL;
    }
    strcpy(question_node->text, question);
    // Set child nodes to NULL at first
    question_node->yes = NULL;
    question_node->no = NULL;

    return question_node;
}

/* TODO 2 */
Node *create_solution_node(const char *solution) {
    if (solution == NULL) return NULL;
    Node* solution_node = (Node*) malloc(sizeof(Node));
    if (solution_node == NULL) return NULL;
    solution_node->isQuestion = 0;
    solution_node->text = (char *) malloc(sizeof(char) * (strlen(solution) + 1));
    if (solution_node->text == NULL){
        free(solution_node);
        return NULL;
    }
    strcpy(solution_node->text, solution);
    solution_node->no = NULL;
    solution_node->yes = NULL;

    return solution_node;
}

/* TODO 3  (recursion allowed) */
void free_tree(Node *node) {
    // For a given node we must first free its leftmost leaf
    // Then we free its rightmost leaft

    // Recusrive:
    // Base cases:
    if (node == NULL){
        return;
    }
    free_tree(node->no);
    free_tree(node->yes);
    free_tree(node);

}

/* TODO 4  (recursion allowed) */
int count_nodes(Node *root) {
    // Base cases (leaf nodes)
    if (root == NULL) return 0;
    // Base case (node)
    else {
        return 1 + count_nodes(root->no) + count_nodes(root->yes);
    }
}


/* ====== FrameStack  (dynamic array, iterative traversal) ======== */

/* TODO 5 */
void fs_init(FrameStack *s) {
    if (s== NULL) return;
    s->capacity = 0;
    s->size = 0;
    s->frames = NULL;

}

/* TODO 6 */
void fs_push(FrameStack *s, Node *node, int answeredYes) {
    if (s== NULL || node == NULL || (answeredYes != 0 && answeredYes != -1 && answeredYes != 1)) return;
    if (s->capacity == s->size){
        if (s->capacity == 0) s->capacity = 2;
        else s->capacity = s->capacity * 2;
        // Dynamically grow Frames array
        s->frames = realloc(s->frames,sizeof(Frame) * s->capacity);
        if (s->frames == NULL) return;
    }
    s->frames[s->size].node = node;
    s->frames[s->size].answeredYes = answeredYes;
    s->size = s->size + 1;

    // When you push to a stack, you first insert then increament/decreament 
    // ex: push: [Frame0] -> [Frame0, Frame1]
    // pop: Frame1
}

/* TODO 7 */
Frame fs_pop(FrameStack *s) {
    Frame dummy = {NULL, -1};
    if (s == NULL || s->size == 0) return dummy;
    Frame frame = s->frames[s->size - 1];
    s->size = s->size - 1;
    return frame;
}

/* TODO 8 */
int fs_empty(FrameStack *s) {
    if (s == NULL || s->size == 0) return 1;
    return 0;

}

/* TODO 9 */
void fs_free(FrameStack *s) {
    if (s == NULL) return;
    free(s->frames);
    s->frames = NULL;
    s->size = 0;
    s->capacity = 0;
}


/* ====== EditStack  (dynamic array, undo/redo) =================== */

/* TODO 10 */
void es_init(EditStack *s) {
}

/* TODO 11 */
void es_push(EditStack *s, Edit e) {
}

/* TODO 12 */
Edit es_pop(EditStack *s) {
    Edit dummy = {0};
    return dummy;
}

/* TODO 13 */
int es_empty(EditStack *s) {
    return 1;
}

/* TODO 14 */
void es_clear(EditStack *s) {
}

/* provided -- do not modify */
void es_free(EditStack *s) {
    free(s->edits);
    s->edits    = NULL;
    s->size     = 0;
    s->capacity = 0;
}

void free_edit_stack(EditStack *s) { es_free(s); }


/* ====== Queue  (linked list, BFS) ============================== */

/* TODO 15 */
void q_init(Queue *q) {
}

/* TODO 16 */
void q_enqueue(Queue *q, Node *node, int id) {
}

/* TODO 17 */
int q_dequeue(Queue *q, Node **node, int *id) {
    return 0;
}

/* TODO 18 */
int q_empty(Queue *q) {
    return 1;
}

/* TODO 19 */
void q_free(Queue *q) {
}


/* ====== Hash table  (separate chaining) ======================== */

/* TODO 20
 * Convert a string to a canonical key:
 *   letters  -> lowercase
 *   spaces   -> underscore
 *   anything else -> drop
 * Caller owns the returned string and must free() it.
 */
char *canonicalize(const char *s) {
    if (s == NULL) return strdup("");
    return NULL;
}

/* TODO 21  (djb2: hash = hash*33 + c, seed 5381) */
unsigned h_hash(const char *s) {
    return 0;
}

/* TODO 22 */
void h_init(Hash *h, int nbuckets) {
}

/* TODO 23 */
int h_put(Hash *h, const char *key, int solutionId) {
    return 0;
}

/* TODO 24 */
int h_contains(const Hash *h, const char *key, int solutionId) {
    return 0;
}

/* TODO 25 */
int *h_get_ids(const Hash *h, const char *key, int *outCount) {
    *outCount = 0;
    return NULL;
}

/* TODO 26 */
void h_free(Hash *h) {
}
