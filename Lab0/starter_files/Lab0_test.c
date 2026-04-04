#include <stdio.h>
#include "Lab0.h"

/*
    Test main to help you debug Lab0. We HIGHLY recommend you write your own test cases to help you debug and test your code.
*/
int main() {
    //Test isPrime()
    int testInput = 3;
    int expectedOutput = 1;
    if(isPrime(testInput) == expectedOutput) {
        printf("isPrime(%d) test passed!\n", testInput);
    } else {
        printf("isPrime test failed. Output was %d, expected %d\n", isPrime(testInput), expectedOutput);
    }
    int isPrimeInput = 8;
    int isPrimeExpectedOutput = 0;
    if(isPrime(isPrimeInput) == isPrimeExpectedOutput) {
        printf("isPrime(%d) test passed!\n", isPrimeInput);
    }
    else {
        printf("isPrime test failed. Output was %d, expected %d\n", isPrime(isPrimeInput), isPrimeExpectedOutput);
    }

    //Test calculateHypotenuse()
    double calculateHypotenuseInput1 = 3.0;
    double calculateHypotenuseInput2 = 4.0;
    double calculateHypotenuseExpectedOutput = 5.0;
    if(calculateHypotenuse(calculateHypotenuseInput1, calculateHypotenuseInput2) == calculateHypotenuseExpectedOutput) {
        printf("calculateHypotenuse test passed!\n");
    }
    else {
        printf("calculateHypotenuse test failed. Output was %lf, expected %lf\n", calculateHypotenuse(calculateHypotenuseInput1, calculateHypotenuseInput2), calculateHypotenuseExpectedOutput);
    }

    //Test gcd()
    int gcdInput1 = -28.0, gcdInput2 = -7.0;
    int gcdExpectedOutput = 7.0;
    if(gcd(gcdInput1, gcdInput2) == gcdExpectedOutput) {
        printf("gcd test passed!\n");
    }
    else {
        printf("gcd test failed. Output was %d, expected %d\n", gcd(gcdInput1, gcdInput2), gcdExpectedOutput);
    }

    return 0;
}
