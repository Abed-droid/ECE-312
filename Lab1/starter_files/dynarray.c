// ECE 312 - Dynamic Array Lab
// Name: Hirad Abedzadeh
// EID:  Ha24847

#include <stdlib.h>
#include "dynarray.h"
#include "errno.h"
/*
 * Creates a new DynamicArray with the specified initial capacity.
 * Returns: Pointer to the new DynamicArray, or NULL on failure
 */
DynamicArray* createArray(int initialCapacity) {
    // return NULL immediately if initicalcapacity is not valid
    if (initialCapacity <=0){
        return NULL;
    }
    // Struct that arr points to is stored on the heap
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (arr == NULL){
        return NULL;
    }
    // Malloc returns a general memory address (void*) to the heap
    arr->data = malloc(sizeof(int) * initialCapacity);
    if (arr->data == NULL){
        // free the struct before retuning NULL
        free(arr);
        return NULL;
    }
    arr->size = 0;

    arr->capacity = initialCapacity;

    // can return arr (pointer) because the memory is on the heap
    return arr;
}

/*
 * Frees all memory associated with the DynamicArray.
 */
void destroyArray(DynamicArray* arr) {
    if (arr == NULL){
        return;
    }
    free(arr->data);
    free(arr);

}

/*
 * Adds an element to the end of the array.
 * Returns: 0 on success, -1 on failure
 */
int addElement(DynamicArray* arr, int value) {
    // TODO: Implement this function
    if (arr == NULL){
        return -1;
    }
    if ((arr->capacity - arr->size) == 0){
        int *temp = realloc(arr->data,((sizeof(int)) * 2* (arr->capacity)));
        if (temp == NULL){
            return -1;
        }
        arr->data = temp;
        arr->capacity = arr->capacity * 2;
    }
    if (arr->size !=0){
        arr->data[arr->size] = value;
        if (arr->data[arr->size] == value){
            arr->size = arr->size + 1;
            return 0;
        }
    }
    arr->data[arr->size] = value;
    arr->size = arr->size + 1;
    return 0;
}

/*
 * Retrieves the element at the specified index.
 * Returns: 0 on success, -1 on failure (invalid index or NULL pointers)
 */
int getElement(DynamicArray* arr, int index, int* result) {
    if (arr == NULL || result == NULL || (arr->size) <= index || index < 0){
        return -1;
    }
    *result = arr->data[index];
    if (*result != arr->data[index]){
        return  -1;
    }
    return 0;
}

/*
 * Sets the element at the specified index to the given value.
 * Returns: 0 on success, -1 on failure
 */
int setElement(DynamicArray* arr, int index, int value) {
    // TODO: Implement this function
    if (arr == NULL || arr->size <= index || index < 0){
        return -1;
    }
    if (arr->data != NULL){
        arr->data[index] = value;
        return 0;
    }
    return -1;
}

/*
 * Returns the current number of elements in the array.
 * Returns -1 if arr is NULL.
 */
int getSize(DynamicArray* arr) {
    // TODO: Implement this function
    if (arr == NULL){
        return -1;
    }
    return (arr->size);
}

/*
 * Returns the current capacity of the array.
 * Returns -1 if arr is NULL.
 */
int getCapacity(DynamicArray* arr) {
    // TODO: Implement this function
    if (arr == NULL){
        return -1;
    }
    return arr->capacity;
}

/*
 * Removes the element at the specified index.
 * Returns: 0 on success, -1 on failure
 */
int removeElement(DynamicArray* arr, int index) {
    // TODO: Implement this function
    if (arr == NULL || arr->size <= index || index < 0){
        return -1;
    }
    // Edge case: what if arr->size is 1? How would we remove?
    // would we just do arr->size - 1?

    // Edge case: what if arr->size == index? How should we remove?
    if ((index +1) == arr->size){
        arr->size = arr->size -1;
        return 0;
    }
    // For arr->size bigger than 1
    for (int i = index; i < arr->size -1 ; i++){
        arr->data[i] = arr->data[i+1];
    }

    arr->size = arr->size -1;
    return 0;
}
