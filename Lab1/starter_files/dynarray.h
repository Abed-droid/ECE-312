typedef struct{
    int* data; // Pointer to heap-allocated array of integers
    int size; // Number of elements currently stored
    int capacity; // Total slots avaliable
} DynamicArray;

DynamicArray* createArray(int initialCapacity);
void destroyArray(DynamicArray* arr);
int addElement(DynamicArray* arr, int value);
int getElement(DynamicArray* arr, int index, int* restult);
int setElement(DynamicArray* arr, int index, int value);
int getSize(DynamicArray* arr);
int getCapacity(DynamicArray* arr);
int removeElement(DynamicArray* arr, int index);
