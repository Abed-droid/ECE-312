#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node {
    int data;           // The value stored in this node
    struct Node* next;  // Pointer to the next node (or NULL if last)
} Node;

typedef struct LinkedList {
    Node* head;   // Pointer to first node (NULL if empty)
    int size;     // Number of nodes in the list
} LinkedList;

LinkedList* createList(void);
void destroyList(LinkedList* list);
int insertAtHead(LinkedList* list, int value);
int insertAtTail(LinkedList* list, int value);
int insertAtIndex(LinkedList* list, int index, int value);
int removeAtHead(LinkedList* list);
int removeAtIndex(LinkedList* list, int index);
int getElement(LinkedList* list, int index, int* result);
int getSize(LinkedList* list);
#endif