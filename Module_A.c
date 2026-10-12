#include "Module_A.h"
#include <stdio.h>


int DEBUG = false;


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }
    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }
    struct timespec start, end;
    long long seconds, nano_seconds;
    /* ToDo 1:
     * Dynamically allocate memory of size ROWS_COUNT x COLUMN_COUNT x size_of(int).
     * run "man malloc" on ubuntu/linux terminal or see online help about malloc function
     */
    size_t s = ROWS_COUNT * COLUMN_COUNT * sizeof(int);
    int* matrix = malloc(s);
    /* ToDo 2:
     * Initialize the allocated memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
    */
    //we can index into it like this
    //memory[i*COLUMN_COUNT+j%ROW_COUNT]
    //it is not necessary to do any sort of preprocessing
    for (int i = 0; i <= ROWS_COUNT-1; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i*COLUMN_COUNT+j] = i+j;
        }
    }
    if (DEBUG) print_matrix_rows(matrix,0, ROWS_COUNT-1);

    //int row_per_process=ROWS_COUNT/num_processes;
    //int start_row=0;
    //int end_row=start_row+row_per_process-1;
    clock_gettime(CLOCK_REALTIME, &start);
     /*
    ToDo 3: Create num_processes count of children processes. (using fork)
    ToDo 4: Ask each child process to print "row_per_process" number of rows by calling function print_matrix_rows(). See header file.
    ToDo 5: Each child process MUST free the dynamically allocated memory for matrix from its own address space after printing the designated rows.
    ToDo 6: All children processes MUST exit after freeing dynamically allocated memory of matrix.
    */
    pid_t* pids = malloc(num_processes*sizeof(pid_t));
    int i = 0;
    for(i; i < num_processes; i++){
        pids[i] = fork();
        if(pids[i] == 0){
            goto CHILD;
        }
    }
    goto PARENT;
    // we just jump out as to not invoke the fork call again 
    // this also has the nice effect of storing 
    // the number that the process is in i
    CHILD:

        if (DEBUG) {printf("(should be 0) process number = %d\n",pids[i]);}
        // this is parent so we wait
        // this is where all the work is done
        int num_rows = ROWS_COUNT/num_processes;
        if (DEBUG) {printf("we print (%d,%d) rows",i*num_rows, (i+1)*num_rows);}
        print_matrix_rows(matrix, i*num_rows, (i+1)*num_rows-1);
        free(matrix);
        free(pids);
        return 0;
    PARENT:
        if (DEBUG) {printf("(ROOT)process number = %d\n",pids[i]);}
        int remainder_rows = ROWS_COUNT % num_processes;
        // if we know that there might be a remainder of rows 
        // if there is we must print the remaining rows to the end

        /*ToDO 7: Parent process must wait for all children processes to complete*/
        i = 0;
        for(i; i < num_processes; i++){
            if (DEBUG) {printf("waiting on process number = %d\n",pids[i]);}
            waitpid(pids[i], NULL, 0);
        }
        if(remainder_rows > 0){
            print_matrix_rows(matrix, ROWS_COUNT-1-remainder_rows, ROWS_COUNT-1);
        }
        printf("all child processes completed their execution\n");
        clock_gettime(CLOCK_REALTIME, &end);
        // Calculate execution time
        seconds=end.tv_sec - start.tv_sec;
        nano_seconds=end.tv_nsec - start.tv_nsec;
        long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
        printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);
        free(matrix);
        free(pids);
        return 0;
}


