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
    NodeMapping *nodes = malloc(sizeof(NodeMapping) * nodeCount); // nodes [NodeMapping0, NodeMapping1,...]
    if (nodes == NULL) {
        fclose(file);
        return 0;
    }
    // BFS traversal to fill the nodes array
    Queue* q = malloc(sizeof(Queue));
    q_init(q);
    q_enqueue(q,g_root,0); // q [0, , , , ....]

    
    // nodes = [NodeMapping0,]
    int index = 0;
    int next_id = 1;
    while(!q_empty(q)){
        Node *CurrentNode;
        int id;
        q_dequeue(q,&CurrentNode, &id); // CurrentNode -> N1
        nodes[index].node = CurrentNode; // CurrentNode 
        nodes[index].id = id;
        index++;
        // nodes = [NodeMapping0,]
        if (CurrentNode->yes != NULL){
            q_enqueue(q,CurrentNode->yes, next_id); // q [1,...]
            next_id++;
        }
        if (CurrentNode->no != NULL){
            q_enqueue(q,CurrentNode->no, next_id); // q [1,1,....]
            next_id++;
        }
    }
    free(q);
    
    // Now we have the nodes in BFS order and their corresponding IDs, we can write the header and the nodes to the file
    uint32_t magic = MAGIC;
    uint32_t version = VERSION;
    uint32_t nodeCount32 = (uint32_t) nodeCount;
    fwrite(&magic, sizeof(uint32_t), 1, file);
    fwrite(&version, sizeof(uint32_t), 1, file);
    fwrite(&nodeCount32, sizeof(uint32_t), 1, file);
    int next_expected_id = 1;
    for (int i = 0; i < nodeCount; i++) {
        Node *node = nodes[i].node;
        uint8_t isQuestion = (uint8_t) node->isQuestion;
        uint32_t textLen = (uint32_t) strlen(node->text);
        int32_t yesId = -1;
        int32_t noId = -1;
        // We need to find the IDs of the yes and no children if they exist
        // Thanks to BFS order, the parent is always at index 'i', and if it has
        // children, their IDs will exactly match their future index in the nodes array.
        // By tracking how many total children have been enqueued before this node's
        // children, we can calculate their exact IDs without a linear search!
        if (node->yes != NULL){
            yesId = next_expected_id;
            next_expected_id++;
        }
        if (node->no != NULL){
            noId = next_expected_id;
            next_expected_id++;
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
    if (filename == NULL) return 0;
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return 0;
    uint32_t magic = NULL;
    uint32_t version = NULL;
    uint32_t nodeCount = NULL;
    fread(&magic, sizeof(uint32_t), 1, file);
    fread(&version, sizeof(uint32_t), 1, file);
    fread(&nodeCount, sizeof(uint32_t), 1, file);
    if (magic != MAGIC || version != VERSION) {
        fclose(file);
        return 0;
    }
    Node **nodes = malloc(sizeof(Node*) * nodeCount); // nodes -> [*Node0, *Node1, ...]
    if (nodes == NULL) {
        fclose(file);
        return 0;
    }
    // We first allocated memory for all Nodes
    for (int i = 0; i < nodeCount; i++) {
        nodes[i] = malloc(sizeof(Node));
        if (nodes[i] == NULL){
            fclose(file);
            return 0;
        }
        nodes[i]->yes = NULL;
        nodes[i]->no = NULL;
    }
    
    // We then iterate throught each node and read from stream
    for (int i  = 0 ; i < nodeCount; i++){
        uint8_t isQuestion;
        uint32_t textLen;

        fread(&(isQuestion), sizeof(uint8_t), 1, file);
        nodes[i]->isQuestion = isQuestion;

        fread(&textLen, sizeof(uint32_t), 1, file);
        nodes[i]->text = malloc(sizeof(char) * (textLen + 1));
        if (nodes[i]->text == NULL) {
            fclose(file);
            return 0;
        }

        fread(nodes[i]->text, sizeof(char), textLen, file);
        nodes[i]->text[textLen] = '\0'; // Add missing null terminator
        int32_t yesId;
        int32_t noId;
        fread(&yesId, sizeof(int32_t), 1, file);
        fread(&noId, sizeof(int32_t), 1, file);

        if (yesId != -1){
            nodes[i]->yes = nodes[yesId];
        }
        if (noId != -1) {
            nodes[i]->no = nodes[noId];
        } 
    }

    // Now replace the old global tree with our new one
    if (g_root != NULL && nodeCount > 0) {
        free_tree(g_root);
    }
    
    if (nodeCount > 0) {
        g_root = nodes[0]; // The root is always the first node (index 0) in BFS
    } else {
        g_root = NULL;
    }

    free(nodes);
    fclose(file);

    return 1;
}