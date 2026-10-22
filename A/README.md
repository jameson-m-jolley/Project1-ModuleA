# Project1-ModuleA

Project 1 - Module A: Matrix processing with multiprocessing

## Overview
This project implements a C program that creates multiple child processes using `fork()`, divides a 16x16 matrix across those processes, and prints assigned rows from the shared matrix. The parent process measures execution time and reports it after all children complete.

## Files included
- `Module_A.h` - header file with matrix constants and the `print_matrix_rows()` helper
- `Module_A.c` - main implementation and process logic
- `Makefile` - builds the program with `gcc`

## Build
From the project directory:

```bash
make
```

## Run
```bash
./Module_A 4
```

The program expects exactly one command-line argument: the number of child processes to create.

## Required behavior
1. Validate command-line input.
   - The program must be called with exactly one argument.
   - The argument must be a positive integer.
2. Dynamically allocate memory for a matrix of size `ROWS_COUNT x COLUMN_COUNT`.
   - `ROWS_COUNT` and `COLUMN_COUNT` are both defined as `16`.
3. Initialize the matrix so each element contains `i + j`, where `i` is the row index and `j` is the column index.
4. Create `num_processes` child processes using `fork()`.
5. Divide the matrix rows among the child processes.
   - Each child should print only its assigned row range by calling `print_matrix_rows()`.
6. Each child must free the dynamically allocated matrix memory before exiting.
7. The parent process must wait for all child processes to finish.
8. Print a completion message and the program execution time.

## Expected output behavior
- Each child prints the rows assigned to it in matrix format using tabs.
- The parent prints:
  - `all child processes completed their execution`
  - `Execution Time with <n> process(es): <time>`

## Notes
- This project uses POSIX system calls like `fork()`, `wait()`, and `clock_gettime()`.
- The supplied `Makefile` compiles the program as `Module_A`.
- The matrix is intended to be processed row-by-row across concurrent child processes.
