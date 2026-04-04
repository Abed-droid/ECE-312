# ECE 312: Lab 0 — Getting Started  - Due Monday January 19th @ 11:59pm

**Course:** ECE 312 – Software Design and Implementation I  
**Instructor:** Evan Speight 
**Purpose:** Workflow familiarization and environment setup

---

## Overview

Welcome to ECE 312! This lab is designed to familiarize you with the workflow you will use for all future programming assignments in this course. While **Lab 0 does not count toward your grade**, you must complete it to ensure you understand the development and submission process.

The skills you practice here—writing code, testing locally, and submitting to Gradescope—will be essential for every lab that follows.

---

## Learning Objectives

By completing this lab, you will:

- Set up your development environment
- Write basic C functions
- Compile and test your code locally
- Submit your solution to Gradescope
- Interpret autograder feedback and iterate on your solution

---

## The Lab Workflow

For this lab and all future labs, follow this three-step process:

### Step 1: Develop Your Solution

Write and test your code on any platform you choose. We **strongly recommend** using the ECE Linux machines (`ssh <your_eid>@linux.ece.utexas.edu`), as this is the environment mirrors the one where your code will be graded. Code that works on your personal machine may behave differently on the grading server due to differences in compilers, libraries, or system configurations.

### Step 2: Submit to Gradescope

Upload your solution file(s) to Gradescope and run the provided test cases. Gradescope will compile your code and execute a suite of automated tests.

### Step 3: Iterate Until You Pass All Tests

Review the feedback from Gradescope, fix any issues, and resubmit. Repeat until all test cases pass.

---

## Important Note for Future Labs

In this lab, you have access to **all** the test cases we use to evaluate your code. However, **in future labs, you will NOT have access to all test cases**. The autograder will run additional hidden tests to verify your solution handles edge cases correctly.

This means you must:

- **Think critically** about edge cases your code might encounter
- **Write your own tests** to verify your implementation
- **Not rely solely** on passing the visible tests

Developing the habit of thorough testing now will save you significant debugging time later in the semester.

---

## Assignment Description

For this lab, you will implement three functions in C. Each function performs a common computational task.

### Files Provided

| File | Description |
|------|-------------|
| `Lab0.h` | Header file containing function declarations (do not modify) |
| `Lab0.c` | Starter file where you will implement your functions |
| `Lab0_test.c` | Sample test file to help you verify your implementations |

### Functions to Implement

You will implement the following three functions in `Lab0.c`:

#### 1. `isPrime`

```c
int isPrime(int x);
```

**Description:** Determines whether an integer is prime.

**Parameters:**
- `x` — the integer to check

**Returns:**
- `1` if `x` is prime
- `0` if `x` is not prime

**Considerations:** Think about how your function should handle edge cases such as negative numbers, zero, and one.

---

#### 2. `calculateHypotenuse`

```c
double calculateHypotenuse(double x, double y);
```

**Description:** Calculates the length of the hypotenuse of a right triangle given the lengths of the two legs.

**Parameters:**
- `x` — length of one leg
- `y` — length of the other leg

**Returns:**
- The length of the hypotenuse (using the Pythagorean theorem: √(x² + y²))
- `-1` if either input is negative
- `0` if either input is zero

**Hint:** You may find the `sqrt()` function from `math.h` useful.

---

#### 3. `gcd`

```c
int gcd(int x, int y);
```

**Description:** Finds the greatest common divisor (GCD) of two integers.

**Parameters:**
- `x` — first integer
- `y` — second integer

**Returns:**
- The greatest common divisor of `x` and `y`

**Considerations:** Your function should handle negative inputs by treating them as their absolute values.

---

## Compiling and Testing Your Code

### Compiling

To compile your code with the provided test file, use:

```bash
gcc -o Lab0_test Lab0.c Lab0_test.c -lm
```

The `-lm` flag links the math library, which is required for the `sqrt()` function.

### Running Tests

Execute your compiled program:

```bash
./Lab0_test
```

You should see output indicating whether each test passed or failed.

### Writing Your Own Tests

The provided `Lab0_test.c` contains only a few sample tests. We **strongly encourage** you to add your own test cases. Consider testing:

- Boundary values (0, 1, negative numbers)
- Large numbers
- Special cases specific to each function

Example of adding a test case to `Lab0_test.c`:

```c
// Additional isPrime test
int testInput = 2;
int expectedOutput = 1;
if(isPrime(testInput) == expectedOutput) {
    printf("isPrime(2) test passed!\n");
} else {
    printf("isPrime(2) test failed.\n");
}
```

---

## Submission Instructions

1. Navigate to the assignments page on Canvas. 
2. Select **Lab0** underneath **Programming Assignments** 
3. Upload your `Lab0.c` file to Gradescope - Note: ONLY UPLOAD `Lab0.c`.  Do no upload any other files.
4. Wait for the autograder to run (this may take a few moments)
5. Review your results and resubmit if necessary

You may submit as many times as you like before the deadline.

---

## Checklist Before Submitting

- [ ] All three functions are implemented
- [ ] Code compiles without errors on the ECE Linux machines
- [ ] All provided test cases pass locally
- [ ] You have tested edge cases with your own tests
- [ ] Your name and EID are filled in at the top of `Lab0.c`

---

## Getting Help

If you encounter issues:

- Attend office hours
- Post questions on Ed Discussion (without sharing code)
- Review lecture notes and textbook materials

---

## Summary

This lab introduces the workflow you will use throughout ECE 312:

1. **Develop** your solution (preferably on ECE Linux machines)
2. **Submit** to Gradescope and run the autograder
3. **Iterate** until all tests pass

Remember: future labs will include hidden test cases, so develop good testing habits now. Writing your own tests is not just encouraged—it's essential for success in this course.

Good luck, and welcome to ECE 312!
