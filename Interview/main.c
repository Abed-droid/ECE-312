// Problem 1 — Signal Peak Detection (most relevant to ADG)
// Given an array of integer sensor readings, return the indices of all "peaks" — a peak is a value strictly greater than both its neighbors. The first and last elements can't be peaks.
// Input:  [1, 3, 2, 5, 4, 7, 6]
// Output: [1, 3, 5]   (values 3, 5, 7)
// Follow-up: What if the data is noisy and you need peaks to be greater than neighbors by at least a threshold t?

// input: An Array of size N
// output: array with peack indecies
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int *peakDetector(int *Array, int size,int *peakCount){
    // allocate memory for new array
    int *ReturnArray = (int*) malloc(sizeof(int) * size); // we must free later
    int j = 0;
    // we first iteratre through the array starting from index 1 to i < 6
    for(int i = 1; i < size - 1; i++){
        // we need to go into the Array at that index and compare it to i-1 and i+1
        // Array = &(Array[0])
        if ((Array[i] > Array[i-1]) && (Array[i] > Array[i+1])){
            ReturnArray[j] = i; // we insert to new Array
            j++;
        }
        
    }
    *peakCount = j;
    return ReturnArray;
}


// "$A*42"
//   XOR: 0x41, but checksum says 0x42
//   → 0 (bad checksum)

// "PING,1*29"
//   Doesn't start with $
//   → 0 (invalid format)

// "$PING129"
//   No * separator
//   → 0 (invalid format)

// "$*00"
//   XOR of nothing = 0x00, checksum "00" = 0x00
//   → 1 (technically valid — edge case worth thinking about)



int checksum(char *Message){
    typedef enum {WAIT,BETWEEN,AFTER} STATES;
    STATES CURRENTSTATE = WAIT;
    char CHECKSUM[3];
    int CHECKSUM_INDEX = 0;
    int Output = 0;
    // STATES NextState = WAIT;
    int runningSUM = 0;
    int CheckSumTotal = 0;


    for (int i = 0; Message[i] != '\0'; i++){
        char character = Message[i];
        switch (CURRENTSTATE){

            case WAIT:
            if(character == '$'){
                CURRENTSTATE = BETWEEN;
            }
            break;

            case BETWEEN:
            if (character == '*'){
                CURRENTSTATE = AFTER;
            }
            else {
            runningSUM ^= character;
            CURRENTSTATE = BETWEEN;
            }
            break;

            case AFTER:
            CHECKSUM[CHECKSUM_INDEX] = character;
            CHECKSUM_INDEX++;
            CURRENTSTATE = AFTER;
            break;
        }
    }
    // pass 2: we compute the int value stored in CHECKSUM and compare to runningSUM
    for (int i = 0; i < 2; i++){
        CheckSumTotal += CHECKSUM[i];
    }
    if (CheckSumTotal == runningSUM) {Output = 1;}
    else {Output = 0;}
    return Output;

    // I need to calculate NextState since its a function of input and CurrentState

}

int main(){
    int *peackCount =  (int*) malloc(sizeof(int) * 1);
    int Array[7] = {1,3,2,5,4,7, 6};

    int *ArrayReturned = peakDetector(Array,7, peackCount);
    assert(*peackCount == 3);
    assert(ArrayReturned[0] == 1);
    assert(ArrayReturned[1] == 3);
    assert(ArrayReturned[2] == 5);
    printf("succus");


    char *Message = "$A*42";

    assert(checksum(Message)==1);


    



}