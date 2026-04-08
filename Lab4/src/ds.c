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
    free(node->text);
    free(node);

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
    if(s == NULL) return;
    s->capacity = 0;
    s->size = 0;
    s->edits = NULL;
}

/* TODO 11 */
void es_push(EditStack *s, Edit e) {
    if (s == NULL) return;
    if (s->capacity == s->size) {
        if (s->capacity == 0) s->capacity = 2;
        else s->capacity = s->capacity * 2;
        s->edits = realloc(s->edits, sizeof(Edit) * s->capacity);
        if (s->edits == NULL) return;
    }
    s->edits[s->size] = e;
    s->size++;
}

/* TODO 12 */
Edit es_pop(EditStack *s) {
    if (s == NULL || s->size == 0) {
        Edit dummy = {0};
        return dummy;
    }
    Edit edit = s->edits[s->size - 1];
    s->size--;
    return edit;
}

/* TODO 13 */
int es_empty(EditStack *s) {
    if (s == NULL || s->size == 0) return 1;
    return 0;
}

/* TODO 14 */
void es_clear(EditStack *s) {
    if (s == NULL) return;
    s->size = 0;
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
    if (q == NULL) return;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

/* TODO 16 */
void q_enqueue(Queue *q, Node *node, int id) {
    if (q == NULL || node == NULL) return;
    QueueNode *newNode = malloc(sizeof(QueueNode));
    if (newNode == NULL) return;
    newNode->treeNode = node;
    newNode->id = id;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = newNode;
    } else {
        q->rear->next = newNode;
    }
    q->rear = newNode;
    q->size++;
}

/* TODO 17 */
int q_dequeue(Queue *q, Node **node, int *id) {
    if (q == NULL || q->size == 0) return 0;
    QueueNode *temp = q->front;
    *node = temp->treeNode;
    *id = temp->id;
    q->front = temp->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;

    return 1;
}

/* TODO 18 */
int q_empty(Queue *q) {
    if (q == NULL || q->size == 0) return 1;
    return 0;
}

/* TODO 19 */
void q_free(Queue *q) {
    if (q == NULL) return;
    QueueNode *current = q->front;
    while (current != NULL) {
        QueueNode *temp = current;
        current = current->next;
        free(temp);
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
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
    char *key = malloc(strlen(s) + 1); // Allocate enough memory for the key
    if (key == NULL) return NULL; // Check for malloc failure
    int j = 0; // Index for the key string
    for (int i = 0; s[i] != '\0'; i++) {
        if (isalpha(s[i])) {    // If it's a letter, convert to lowercase and add to key
            key[j++] = tolower(s[i]);
        } else if (isspace(s[i])) { // If it's a space, add an underscore
            key[j++] = '_';
        }
    }
    key[j] = '\0'; // Null-terminate the key string
    return key;
}

/* TODO 21  (djb2: hash = hash*33 + c, seed 5381) */
unsigned h_hash(const char *s) {
    if (s == NULL) return 0;
    unsigned hash = 5381;
    for (int i = 0; s[i] != '\0'; i++) {
        hash = ((hash << 5) + hash) + (unsigned char)s[i]; // hash * 33 + c
    }
    return hash;
}

/* TODO 22 */
void h_init(Hash *h, int nbuckets) {
    if (h == NULL || nbuckets <= 0) return;
    h->nbuckets = nbuckets;
    h->size = 0;
    h->buckets = malloc(sizeof(Entry*) * nbuckets);
    if (h->buckets == NULL) return;
    for (int i = 0; i < nbuckets; i++) {
        h->buckets[i] = NULL;
    }
}

/* TODO 23 */
int h_put(Hash *h, const char *key, int solutionId) {
    if (h == NULL || key == NULL) return 0;
    char *canonicalKey = canonicalize(key);
    if (canonicalKey == NULL) return 0;
    unsigned hashValue = h_hash(canonicalKey);
    int bucketIndex = hashValue % h->nbuckets;
    Entry *current = h->buckets[bucketIndex];
    while (current != NULL) {
        if (strcmp(current->key, canonicalKey) == 0) {
            // Key already exists, add solutionId to vals if not already present
            for (int i = 0; i < current->vals.count; i++) {
                if (current->vals.ids[i] == solutionId) {
                    free(canonicalKey);
                    return 1;
                }
            }
            if (current->vals.capacity == current->vals.count) {
                if (current->vals.capacity == 0) current->vals.capacity = 2;
                else current->vals.capacity = current->vals.capacity * 2;
                current->vals.ids = realloc(current->vals.ids, sizeof(int) * current->vals.capacity);
                if (current->vals.ids == NULL) return 0;
            }
            current->vals.ids[current->vals.count] = solutionId;
            current->vals.count++;
            
            free(canonicalKey);
            return 1;
        }
        current = current->next;
    }

    // Key does not exist, create a new entry
    Entry *newEntry = malloc(sizeof(Entry));
    if (newEntry == NULL) {
        free(canonicalKey);
        return 0;
    }
    newEntry->key = canonicalKey;
    
    newEntry->vals.capacity = 2;
    newEntry->vals.ids = malloc(sizeof(int) * newEntry->vals.capacity);
    if (newEntry->vals.ids == NULL) {
        free(canonicalKey);
        free(newEntry);
        return 0;
    }
    
    newEntry->vals.ids[0] = solutionId;
    newEntry->vals.count = 1;
    newEntry->next = h->buckets[bucketIndex];
    h->buckets[bucketIndex] = newEntry;
    h->size++;

    return 1;

}

/* TODO 24 */
int h_contains(const Hash *h, const char *key, int solutionId) {
    if (h == NULL || key == NULL) return 0;
    char *canonicalKey = canonicalize(key);
    if (canonicalKey == NULL) return 0;
    unsigned hashValue = h_hash(canonicalKey);
    int bucketIndex = hashValue % h->nbuckets;
    Entry *current = h->buckets[bucketIndex];
    while (current != NULL) {
        if (strcmp(current->key, canonicalKey) == 0) {
            for (int i = 0; i < current->vals.count; i++) {
                if (current->vals.ids[i] == solutionId) {
                    free(canonicalKey);
                    return 1; // Found the solutionId for the key
                }
            }
        }
        current = current->next;
    }
    free(canonicalKey);
    return 0; // Key or solutionId not found
}

/* TODO 25 */
int *h_get_ids(const Hash *h, const char *key, int *outCount) {
    *outCount = 0;
    if (h == NULL || key == NULL) return NULL;
    char *canonicalKey = canonicalize(key);
    if (canonicalKey == NULL) return NULL;
    unsigned hashValue = h_hash(canonicalKey);
    int bucketIndex = hashValue % h->nbuckets;
    Entry *current = h->buckets[bucketIndex];
    while (current != NULL) {
        if (strcmp(current->key, canonicalKey) == 0) {
            *outCount = current->vals.count;
            int *ids = malloc(sizeof(int) * *outCount);
            if (ids == NULL) {
                free(canonicalKey);
                return NULL;
            }
            for (int i = 0; i < current->vals.count; i++) {
                ids[i] = current->vals.ids[i];
            }
            free(canonicalKey);
            return ids;
        }
        current = current->next;
    }
    free(canonicalKey);
    return NULL;
}

/* TODO 26 */
void h_free(Hash *h) {
    if (h == NULL) return;
    for (int i = 0; i < h->nbuckets; i++) {
        Entry *current = h->buckets[i];
        while (current != NULL) {
            Entry *next = current->next;
            free(current->key);
            free(current->vals.ids);
            free(current);
            current = next;
        }
    }
    free(h->buckets);
    h->buckets = NULL;
    h->size = 0;
}