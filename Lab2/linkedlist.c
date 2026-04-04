// ECE 312 - Linked List Lab
// Name: Hirad Abedzadeh
// EID:  Ha24847

#include <stdlib.h>
#include "linkedlist.h"
#include <stdint.h>

/*
 * Creates a new empty linked list.
 * 
 * Returns: Pointer to the new LinkedList, or NULL on failure
 */
LinkedList* createList(void) {
    // TODO: Implement this function
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    if (list == NULL){
        return NULL;
    }
    list->size = 0;
    list->head = NULL;
    return list;
}

/*
 * Frees all memory associated with the linked list.
 * 
 * Returns: Nothing 
 */
void destroyList(LinkedList* list) {
    // TODO: Implement this function
    if (list == NULL){
        return;
    }
    Node* current_node = NULL;
    Node* p = NULL;
    current_node = list->head;
    // Case 1: list->head is empty, meaning there are no nodes. Only need to free list struct
    if (list->head == NULL){
        free(list);
        return;
    }
    // Case 2: there are nodes in the list
    // We need to traverse thorugh the list and free each node then free the list
    while(current_node != NULL){
        p = current_node->next;
        free(current_node);
        current_node = p;
    }
    
    free(list);
}

/*
 * Inserts a new element at the front (head) of the list.
 * 
 * Returns: 0 on success, -1 on failure
 */
int insertAtHead(LinkedList* list, int value) {
    if (list == NULL){
        return -1;
    }
    Node* node = (Node *) malloc(sizeof(Node));
    if (node == NULL){
        return -1;
    }
    node->data = value;
    node->next = list->head;
    list->head = node;
    list->size = list->size +1;
    // TODO: Implement this function
    return 0;
}

/*
 * Inserts a new element at the end (tail) of the list.
 * 
 * Returns: 0 on success, -1 on failure
 */
int insertAtTail(LinkedList* list, int value) {
    // TODO: Implement this function
    if (list == NULL){
        return -1;
    }
    Node* node = (Node*) malloc(sizeof(Node));
    if (node == NULL){
        return -1;
    }
    // Case 1: empty list.
    if (list->size == 0 && list->head == NULL){
        list->head = node;
        node->data = value;
        node->next = NULL;
        list->size = list->size + 1;
        return 0;
    }
    // Case 2: non-empty lisst
    Node* current_node = list->head;
    Node* insert_after = NULL;
    while (current_node != NULL){
        insert_after = current_node;
        current_node = current_node->next;
        
    }
    insert_after->next = node;
    node->data = value;
    node->next = NULL;
    list->size = list->size + 1;
    return 0;
}

/*
 * Inserts a new element at the specified index.
 *
 * Returns: 0 on success, -1 if index invalid or allocation fails
 */
int insertAtIndex(LinkedList* list, int index, int value) {
    // TODO: Implement this function
    
    // Consider: What if index == 0? What if index == size?
    // Can you reuse insertAtHead or insertAtTail?
    if (list == NULL || index < 0 || index > list->size){
        return -1;
    }
    // Case 1: empty list and index is 0
    if (index == 0){
        int result = insertAtHead(list, value);
        return result;

    }
    // Case 2: non-empty and index == size
    if(list->size == index){
        int result = insertAtTail(list, value);
        return result;
    }
    // Case 3: non-empty and index in middle
    Node* node = (Node*) malloc(sizeof(Node));
    if (node == NULL){
        return -1;
    }
    Node* old_node = list->head;
    Node* p = NULL;
    for(int i = 0; i < index; i++){
        p = old_node;
        old_node = old_node->next;
    }
    p->next = node;
    node->data = value;
    node->next = old_node;
    list->size = list->size + 1;
    return 0;
}

/*
 * Removes the element at the front (head) of the list.
 *
 * Returns: 0 on success, -1 if list is empty
 */
int removeAtHead(LinkedList* list) {
    // Case 1: invalid list/empty list
    if (list == NULL || list->head == NULL || list->size == 0){
        return -1;
    }
    Node* head_node = list->head;
    Node * p = NULL;
    p = head_node->next;
    list->head = p;
    list->size = list->size -1;
    free(head_node);
    return 0;
}

/*
 * Removes the element at the specified index.
 * 
 * Returns: 0 on success, -1 if index is out of bounds
 */
int removeAtIndex(LinkedList* list, int index) {
    // TODO: Implement this function
    // Consider: What if index == 0? Can you reuse removeAtHead?
    if (list == NULL || index < 0 || index >= list->size || (list->head == NULL && list->size == 0)){
        return -1;
    }
    if (index == 0){
        int result = removeAtHead(list);
        return result;
    }
    // Case 1: index for last node
    if (index == list->size-1){
        Node* node_to_remove = list->head;
        Node* p = NULL;
        for(int i = 0; i < index; i++){
            p = node_to_remove;
            node_to_remove = node_to_remove->next;
        }
        p->next = NULL;
        free(node_to_remove);
        list->size = list->size -1;
        return 0;
    }
    // Case 2: index for middle node;
    Node* node_to_remove = list->head;
    Node* p = NULL;
    for(int i = 0; i < index; i++){
        p = node_to_remove;
        node_to_remove = node_to_remove->next;
    }
    p->next = node_to_remove->next;
    free(node_to_remove);
    list->size = list->size -1;
    return 0;
}

/*
 * Retrieves the element at the specified index.
 *
 * Returns: 0 on success, -1 if index out of bounds or NULL pointers
 */
int getElement(LinkedList* list, int index, int* result) {
    // TODO: Implement this function
    if (list == NULL || index >= list->size || index < 0 || list->size == 0 || list->head == NULL || result == NULL){
        return -1;
    }
    Node* current_node = list->head;
    for (int i = 0; i < index; i++){
        current_node = current_node->next;
    }
    *result = current_node->data;
    return 0;
    
}

/*
 * Returns the number of elements in the list.
 *
 * Returns -1 if list is NULL.
 */
int getSize(LinkedList* list) {
    // TODO: Implement this function
    if (list == NULL){
        return -1;
    }
    return list->size;
    
}
