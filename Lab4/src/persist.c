#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lab4.h"

extern Node *g_root;

#define MAGIC   0x54454348u   /* "TECH" */
#define VERSION 1u

typedef struct {
    Node *node;
    int id;
} NodeMapping;

/* ----------------------------------------------------------------
 * TODO 27  save_tree
 *
 * Serialize the entire tree to a binary file using BFS order.
 *
 * File format:
 *   Header:  uint32 magic | uint32 version | uint32 nodeCount
 *   Per node (BFS order):
 *     uint8  isQuestion
 *     uint32 textLen          (bytes, no null terminator in file)
 *     char[] text             (exactly textLen bytes)
 *     int32  yesId            (-1 if NULL)
 *     int32  noId             (-1 if NULL)
 *
 * Return 1 on success, 0 on failure.
 * ---------------------------------------------------------------- */
int save_tree(const char *filename) {
    if (filename == NULL) return 0;
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return 0;
    // We will first do a BFS traversal to count the number of nodes and store them in an array
    int nodeCount = count_nodes(g_root);
    NodeMapping *nodes = malloc(sizeof(NodeMapping) * nodeCount);
    if (nodes == NULL) {
        fclose(file);
        return 0;
    }
    // BFS traversal to fill the nodes array
    Queue q;
    q_init(&q);
    q_enqueue(&q, g_root, 0);
    int index = 0;
    // We will also need to keep track of the IDs of the nodes as we traverse
    while (!q_empty(&q)) {
        Node *currentNode;
        int id;
        q_dequeue(&q, &currentNode, &id);
        nodes[index].node = currentNode;
        nodes[index].id = id;
        index++;
        if (currentNode->yes != NULL) {
            q_enqueue(&q, currentNode->yes, index);
        }
        if (currentNode->no != NULL) {
            q_enqueue(&q, currentNode->no, index);
        }
    }
    // Now we have the nodes in BFS order and their corresponding IDs, we can write the header and the nodes to the file
    uint32_t magic = MAGIC;
    uint32_t version = VERSION;
    uint32_t nodeCount32 = (uint32_t) nodeCount;
    fwrite(&magic, sizeof(uint32_t), 1, file);
    fwrite(&version, sizeof(uint32_t), 1, file);
    fwrite(&nodeCount32, sizeof(uint32_t), 1, file);
    for (int i = 0; i < nodeCount; i++) {
        Node *node = nodes[i].node;
        uint8_t isQuestion = (uint8_t) node->isQuestion;
        uint32_t textLen = (uint32_t) strlen(node->text);
        int32_t yesId = -1;
        int32_t noId = -1;
        // We need to find the IDs of the yes and no children if they exist
        for (int j = 0; j < nodeCount; j++) {
            if (nodes[j].node == node->yes) {
                yesId = nodes[j].id;
            }
            if (nodes[j].node == node->no) {
                noId = nodes[j].id;
            }
        }
        fwrite(&isQuestion, sizeof(uint8_t), 1, file);
        fwrite(&textLen, sizeof(uint32_t), 1, file);
        fwrite(node->text, sizeof(char), textLen, file);
        fwrite(&yesId, sizeof(int32_t), 1, file);
        fwrite(&noId, sizeof(int32_t), 1, file);
    }
    free(nodes);
    fclose(file);


    return 1;
}

/* ----------------------------------------------------------------
 * TODO 28  load_tree
 *
 * Read a file written by save_tree and reconstruct the tree.
 * Validate the magic number.  Read all nodes into a flat array
 * first, then link children in a second pass.
 * Free any existing g_root before installing the new one.
 * Return 1 on success, 0 on any error (free partial allocations).
 * ---------------------------------------------------------------- */
int load_tree(const char *filename) {
    return 0;
}
