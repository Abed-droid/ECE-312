#include <stdio.h>
#include "dynarray.h"

/*
 * Sample test file for the Dynamic Array lab.
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

void test_createArray() {
    printf("\n=== Testing createArray ===\n");
    
    // Test 1: Basic creation
    DynamicArray* arr = createArray(5);
    if (arr == NULL) {
        test_fail("Basic creation", "returned NULL");
        return;
    }
    if (getCapacity(arr) != 5) {
        test_fail("Basic creation", "capacity not set correctly");
    } else if (getSize(arr) != 0) {
        test_fail("Basic creation", "size should be 0");
    } else {
        test_pass("Basic creation");
    }
    destroyArray(arr);
    
    // Test 2: Invalid capacity
    arr = createArray(0);
    if (arr != NULL) {
        test_fail("Zero capacity", "should return NULL");
        destroyArray(arr);
    } else {
        test_pass("Zero capacity");
    }
    
    arr = createArray(-5);
    if (arr != NULL) {
        test_fail("Negative capacity", "should return NULL");
        destroyArray(arr);
    } else {
        test_pass("Negative capacity");
    }
}

void test_addElement() {
    printf("\n=== Testing addElement ===\n");
    
    DynamicArray* arr = createArray(2);
    if (arr == NULL) {
        test_fail("Setup", "createArray failed");
        return;
    }
    
    // Test 1: Add single element
    if (addElement(arr, 42) != 0) {
        test_fail("Add single element", "returned failure");
    } else if (getSize(arr) != 1) {
        test_fail("Add single element", "size not updated");
    } else {
        test_pass("Add single element");
    }
    
    // Test 2: Add to capacity
    addElement(arr, 43);
    if (getSize(arr) != 2) {
        test_fail("Add to capacity", "size incorrect");
    } else {
        test_pass("Add to capacity");
    }
    
    // Test 3: Add beyond capacity (should trigger resize)
    int old_capacity = getCapacity(arr);
    if (addElement(arr, 44) != 0) {
        test_fail("Add with resize", "returned failure");
    } else if (getCapacity(arr) <= old_capacity) {
        test_fail("Add with resize", "capacity did not increase");
    } else if (getSize(arr) != 3) {
        test_fail("Add with resize", "size incorrect after resize");
    } else {
        test_pass("Add with resize");
    }
    
    // Test 4: Verify elements are preserved after resize
    int val;
    int success = 1;
    if (getElement(arr, 0, &val) != 0 || val != 42) success = 0;
    if (getElement(arr, 1, &val) != 0 || val != 43) success = 0;
    if (getElement(arr, 2, &val) != 0 || val != 44) success = 0;
    if (success) {
        test_pass("Elements preserved after resize");
    } else {
        test_fail("Elements preserved after resize", "values corrupted");
    }
    
    destroyArray(arr);
}

void test_getElement() {
    printf("\n=== Testing getElement ===\n");
    
    DynamicArray* arr = createArray(5);
    if (arr == NULL) {
        test_fail("Setup", "createArray failed");
        return;
    }
    
    addElement(arr, 10);
    addElement(arr, 20);
    addElement(arr, 30);
    
    int result;
    
    // Test 1: Get valid elements
    if (getElement(arr, 0, &result) != 0 || result != 10) {
        test_fail("Get first element", "incorrect value");
    } else {
        test_pass("Get first element");
    }
    
    if (getElement(arr, 2, &result) != 0 || result != 30) {
        test_fail("Get last element", "incorrect value");
    } else {
        test_pass("Get last element");
    }
    
    // Test 2: Out of bounds
    if (getElement(arr, 3, &result) != -1) {
        test_fail("Out of bounds (index = size)", "should return -1");
    } else {
        test_pass("Out of bounds (index = size)");
    }
    
    if (getElement(arr, -1, &result) != -1) {
        test_fail("Negative index", "should return -1");
    } else {
        test_pass("Negative index");
    }
    
    destroyArray(arr);
}

void test_setElement() {
    printf("\n=== Testing setElement ===\n");
    
    DynamicArray* arr = createArray(5);
    if (arr == NULL) {
        test_fail("Setup", "createArray failed");
        return;
    }
    
    addElement(arr, 10);
    addElement(arr, 20);
    
    // Test 1: Set valid element
    if (setElement(arr, 0, 100) != 0) {
        test_fail("Set element", "returned failure");
    } else {
        int result;
        getElement(arr, 0, &result);
        if (result != 100) {
            test_fail("Set element", "value not updated");
        } else {
            test_pass("Set element");
        }
    }
    
    // Test 2: Set out of bounds
    if (setElement(arr, 5, 999) != -1) {
        test_fail("Set out of bounds", "should return -1");
    } else {
        test_pass("Set out of bounds");
    }
    
    destroyArray(arr);
}

void test_removeElement() {
    printf("\n=== Testing removeElement ===\n");
    
    DynamicArray* arr = createArray(5);
    if (arr == NULL) {
        test_fail("Setup", "createArray failed");
        return;
    }
    
    addElement(arr, 10);
    addElement(arr, 20);
    addElement(arr, 30);
    addElement(arr, 40);
    
    // Test 1: Remove middle element
    if (removeElement(arr, 1) != 0) {
        test_fail("Remove middle", "returned failure");
    } else if (getSize(arr) != 3) {
        test_fail("Remove middle", "size not decremented");
    } else {
        int val;
        getElement(arr, 0, &val);
        int ok = (val == 10);
        getElement(arr, 1, &val);
        ok = ok && (val == 30);
        getElement(arr, 2, &val);
        ok = ok && (val == 40);
        if (ok) {
            test_pass("Remove middle");
        } else {
            test_fail("Remove middle", "elements not shifted correctly");
        }
    }
    
    // Test 2: Remove first element
    if (removeElement(arr, 0) != 0) {
        test_fail("Remove first", "returned failure");
    } else {
        int val;
        getElement(arr, 0, &val);
        if (val == 30 && getSize(arr) == 2) {
            test_pass("Remove first");
        } else {
            test_fail("Remove first", "incorrect result");
        }
    }
    // Own test: Remove last element
    if (removeElement(arr, 1) != 0){
        test_fail("Remove last", "returned failure");
    } else {
        int val;
        int result;
        result = getElement(arr, 1, &val);
        if (result == -1){
            test_pass("Remove Last");
        }
        else{
            test_fail("Remove last", "Incorrect Result");
        }
    }
    // Test 3: Remove out of bounds
    if (removeElement(arr, 10) != -1) {
        test_fail("Remove out of bounds", "should return -1");
    } else {
        test_pass("Remove out of bounds");
    }
    
    destroyArray(arr);
}

void test_null_handling() {
    printf("\n=== Testing NULL handling ===\n");
    
    // These should not crash!
    destroyArray(NULL);
    test_pass("destroyArray(NULL) doesn't crash");
    
    if (addElement(NULL, 5) == -1) {
        test_pass("addElement(NULL, ...) returns -1");
    } else {
        test_fail("addElement(NULL, ...)", "should return -1");
    }
    
    int result;
    if (getElement(NULL, 0, &result) == -1) {
        test_pass("getElement(NULL, ...) returns -1");
    } else {
        test_fail("getElement(NULL, ...)", "should return -1");
    }
    
    if (getSize(NULL) == -1) {
        test_pass("getSize(NULL) returns -1");
    } else {
        test_fail("getSize(NULL)", "should return -1");
    }
}
void test_remove_all_elements() {
    DynamicArray* arr = createArray(3);
    addElement(arr, 1);
    addElement(arr, 2);
    addElement(arr, 3);
    
    // Remove all elements from the end
    while (getSize(arr) > 0) {
        removeElement(arr, getSize(arr) - 1);
    }
    
    if (getSize(arr) == 0) {
        printf("test_remove_all_elements PASSED\n");
    } else {
        printf("test_remove_all_elements FAILED\n");
    }
    
    destroyArray(arr);
}
int main() {
    printf("========================================\n");
    printf("  Dynamic Array Lab - Test Suite\n");
    printf("========================================\n");
    
    test_createArray();
    test_addElement();
    test_getElement();
    test_setElement();
    test_remove_all_elements();
    test_removeElement();
    test_null_handling();
    
    
    printf("\n========================================\n");
    printf("  Results: %d passed, %d failed\n", tests_passed, tests_failed);
    printf("========================================\n");
    
    if (tests_failed > 0) {
        printf("\nNote: Write your own tests to check additional edge cases!\n");
    }
    
    return tests_failed;
}
