// ECE 312 Lab 0
// Hirad Abedzadeh
// ha24847
// Spring 2026

#include "Lab0.h"
#include "math.h"
#include "stdint.h"
#include "string.h"


/*
    Write a function that takes in an integer x and returns 1 if x is prime and 0 if x is not prime.
*/
int isPrime(int x) {
    if (x < 2){
        return 0;
    }
    for (int i = 1; i <= x; i++){
        if ((x % i == 0) && (i != x && i != 1)){
            return 0;
        }
    }
    return 1;
}

/*
    Write a function that takes in two legs of a right triange, x and y, and returns the length of the hypotenuse.
*/
double calculateHypotenuse(double x, double y) {
    if (x == 0 || y == 0){
        return 0;
    }
    else if (x <0 || y <0){
        return -1;
    }
    return sqrt(fabs(x*x) + fabs(y*y));
}

/*
    Write a function that finds the greatest common divisor of two integers x and y.
*/
int gcd(int x, int y) {
    uint32_t r;
    x = fabs(x);
    y = fabs(y);
    
    
    if (x % y == 0){
        return y;
    }
    while (x % y != 0){
        r = x % y;
        x = y;
        y = r;
    }
    return r;
}
