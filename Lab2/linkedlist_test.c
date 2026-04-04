#include <stdio.h>
#include "linkedlist.h"

/*
 * Sample test file for the Linked List lab.
 * 
 * These tests cover basic functionality. You should write additional
 * tests to verify edge cases and error handling!
 */

int tests_passed = 0;
int tests_failed = 0;

void test_pass(const char* test_name) {
    printf("  [PASS] %s\n", test_name);
    tests_passed++;
}

void test_fail(const char* test_name, const char* message) {
    printf("  [FAIL] %s - %s\n", test_name, message);
    tests_failed++;
}

// Helper function to print list contents (useful for debugging)
void printList(LinkedList* list) {
    if (list == NULL) {
        printf("(null list)\n");
        return;
    }
    printf("List (size=%d): head -> ", list->size);
    Node* current = list->head;
    while (current != NULL) {
        printf("[%d] -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void test_createList() {
    printf("\n=== Testing createList ===\n");
    
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Create empty list", "returned NULL");
        return;
    }
    
    if (list->head != NULL) {
        test_fail("Create empty list", "head should be NULL");
    } else if (getSize(list) != 0) {
        test_fail("Create empty list", "size should be 0");
    } else {
        test_pass("Create empty list");
    }
    printList(list);
    destroyList(list);
 }

void test_insertAtHead() {
    printf("\n=== Testing insertAtHead ===\n");
    
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Setup", "createList failed");
        return;
    }
    printList(list);
    // Insert first element
    if (insertAtHead(list, 10) != 0) {
        printList(list);
        test_fail("Insert into empty list", "returned failure");
    } else if (getSize(list) != 1) {
        printList(list);
        test_fail("Insert into empty list", "size should be 1");
    } else {
        printList(list);
        int val;
        getElement(list, 0, &val);
        if (val == 10) {
            test_pass("Insert into empty list");
        } else {
            test_fail("Insert into empty list", "wrong value");
        }
    }
    printList(list);
    
    // Insert second element at head
    insertAtHead(list, 20);
    int val;
    getElement(list, 0, &val);
    if (val == 20 && getSize(list) == 2) {
        test_pass("Insert at head of non-empty list");
    } else {
        test_fail("Insert at head of non-empty list", "wrong value or size");
    }
    printList(list);
    // Verify order: should be [20, 10]
    int first, second;
    getElement(list, 0, &first);
    getElement(list, 1, &second);
    if (first == 20 && second == 10) {
        test_pass("Correct order after head insertions");
    } else {
        test_fail("Correct order after head insertions", "elements in wrong order");
    }
    printList(list);
    destroyList(list);
}

void test_insertAtTail() {
    printf("\n=== Testing insertAtTail ===\n");
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Setup", "createList failed");
        return;
    }
    
    // Insert into empty list
    if (insertAtTail(list, 10) != 0) {
        test_fail("Insert into empty list", "returned failure");
    } else if (getSize(list) != 1) {
        test_fail("Insert into empty list", "size should be 1");
    } else {
        printList(list);
        test_pass("Insert into empty list");
    }
    
    // Insert more elements

    insertAtTail(list, 20);
    printList(list);
    insertAtTail(list, 30);
    printList(list);
    
    // Verify order: should be [10, 20, 30]
    int a, b, c;
    getElement(list, 0, &a);
    getElement(list, 1, &b);
    getElement(list, 2, &c);
    
    if (a == 10 && b == 20 && c == 30 && getSize(list) == 3) {
        test_pass("Correct order after tail insertions");
    } else {
        test_fail("Correct order after tail insertions", "elements in wrong order");
    }
    printList(list);
    
    destroyList(list);
}

void test_insertAtIndex() {
    printf("\n=== Testing insertAtIndex ===\n");
    
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Setup", "createList failed");
        return;
    }
    
    // Insert at index 0 (empty list)
    if (insertAtIndex(list, 0, 20) != 0) {
        test_fail("Insert at index 0 (empty)", "returned failure");
    } else {
        printList(list);
        test_pass("Insert at index 0 (empty)");
    }
    
    // Insert at index 0 (becomes new head)
    insertAtIndex(list, 0, 10);
    printList(list);
    
    // Insert at end (index == size)
    insertAtIndex(list, 2, 40);
    printList(list);
    
    // Insert in middle
    insertAtIndex(list, 2, 30);
    printList(list);
    
    // Should now be [10, 20, 30, 40]
    int vals[4];
    for (int i = 0; i < 4; i++) {
        getElement(list, i, &vals[i]);
    }
    
    if (vals[0] == 10 && vals[1] == 20 && vals[2] == 30 && vals[3] == 40) {
        test_pass("Insert at various indices");
    } else {
        test_fail("Insert at various indices", "elements in wrong positions");
        printList(list);
    }
    
    // Test invalid index
    if (insertAtIndex(list, -1, 99) == -1 && insertAtIndex(list, 10, 99) == -1) {
        test_pass("Reject invalid indices");
    } else {
        test_fail("Reject invalid indices", "should return -1");
    }
    
    destroyList(list);
}

void test_removeAtHead() {
    printf("\n=== Testing removeAtHead ===\n");
    
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Setup", "createList failed");
        return;
    }
    
    // Remove from empty list
    if (removeAtHead(list) == -1) {
        test_pass("Remove from empty list returns -1");
    } else {
        test_fail("Remove from empty list", "should return -1");
    }
    
    // Add elements and remove
    printList(list);
    insertAtTail(list, 10);
    printList(list);
    insertAtTail(list, 20);
    printList(list);
    insertAtTail(list, 30);
    printList(list);
    
    if (removeAtHead(list) != 0) {
        test_fail("Remove head", "returned failure");
    } else {
        int val;
        printList(list);
        getElement(list, 0, &val);
        if (val == 20 && getSize(list) == 2) {
            test_pass("Remove head");
        } else {
            test_fail("Remove head", "wrong value or size after removal");
        }
    }
    
    destroyList(list);
}

void test_removeAtIndex() {
    printf("\n=== Testing removeAtIndex ===\n");
    
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Setup", "createList failed");
        return;
    }
    printList(list);
    insertAtTail(list, 10);
    printList(list);
    insertAtTail(list, 20);
    printList(list);
    insertAtTail(list, 30);
    printList(list);
    insertAtTail(list, 40);
    printList(list);
    // List: [10, 20, 30, 40]
    
    // Remove middle element (index 1)
    if (removeAtIndex(list, 1) != 0) {
        test_fail("Remove middle element", "returned failure");
    } else {
        // Should be [10, 30, 40]
        int a, b, c;
        getElement(list, 0, &a);
        getElement(list, 1, &b);
        getElement(list, 2, &c);
        if (a == 10 && b == 30 && c == 40 && getSize(list) == 3) {
            printList(list);
            test_pass("Remove middle element");
        } else {
            test_fail("Remove middle element", "wrong values or size");
        }
    }
    
    // Remove last element
    if (removeAtIndex(list, 2) != 0) {
        printList(list);
        test_fail("Remove last element", "returned failure");
    } else if (getSize(list) == 2) {
        printList(list);
        test_pass("Remove last element");
    } else {
        printList(list);
        test_fail("Remove last element", "wrong size");
    }
    
    // Test out of bounds
    if (removeAtIndex(list, 5) == -1 && removeAtIndex(list, -1) == -1) {
        printList(list);
        test_pass("Reject out of bounds removal");
    } else {
        test_fail("Reject out of bounds removal", "should return -1");
    }
    
    destroyList(list);
}

void test_getElement() {
    printf("\n=== Testing getElement ===\n");
    
    LinkedList* list = createList();
    if (list == NULL) {
        test_fail("Setup", "createList failed");
        return;
    }
    
    insertAtTail(list, 100);
    insertAtTail(list, 200);
    insertAtTail(list, 300);
    
    int result;
    
    // Get first element
    if (getElement(list, 0, &result) == 0 && result == 100) {
        test_pass("Get first element");
    } else {
        test_fail("Get first element", "wrong value");
    }
    
    // Get last element
    if (getElement(list, 2, &result) == 0 && result == 300) {
        test_pass("Get last element");
    } else {
        test_fail("Get last element", "wrong value");
    }
    
    // Out of bounds
    if (getElement(list, 3, &result) == -1 && getElement(list, -1, &result) == -1) {
        test_pass("Reject out of bounds access");
    } else {
        test_fail("Reject out of bounds access", "should return -1");
    }
    
    destroyList(list);
}

void test_null_handling() {
    printf("\n=== Testing NULL handling ===\n");
    
    // These should not crash!
    destroyList(NULL);
    test_pass("destroyList(NULL) doesn't crash");
    
    if (insertAtHead(NULL, 5) == -1) {
        test_pass("insertAtHead(NULL, ...) returns -1");
    } else {
        test_fail("insertAtHead(NULL, ...)", "should return -1");
    }
    
    if (insertAtTail(NULL, 5) == -1) {
        test_pass("insertAtTail(NULL, ...) returns -1");
    } else {
        test_fail("insertAtTail(NULL, ...)", "should return -1");
    }
    
    if (getSize(NULL) == -1) {
        test_pass("getSize(NULL) returns -1");
    } else {
        test_fail("getSize(NULL)", "should return -1");
    }
    
    int result;
    if (getElement(NULL, 0, &result) == -1) {
        test_pass("getElement(NULL, ...) returns -1");
    } else {
        test_fail("getElement(NULL, ...)", "should return -1");
    }
}

int main() {
    printf("========================================\n");
    printf("  Linked List Lab - Test Suite\n");
    printf("========================================\n");
    
    test_createList();
    test_insertAtHead();
    test_insertAtTail();
    test_insertAtIndex();
    test_removeAtHead();
    test_removeAtIndex();
    test_getElement();
    test_null_handling();
    
    printf("\n========================================\n");
    printf("  Results: %d passed, %d failed\n", tests_passed, tests_failed);
    printf("========================================\n");
    
    if (tests_failed > 0) {
        printf("\nTip: Use the printList() helper function to debug!\n");
        printf("Write your own tests to check additional edge cases!\n");
    }
    
    return tests_failed;
}
