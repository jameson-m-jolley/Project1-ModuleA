#include "Project1_Module_B.h"

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

    int shmid_matrix = shmget(IPC_PRIVATE, ROWS_COUNT * COLUMN_COUNT * sizeof(int), IPC_CREAT | 0666);
    int *matrix = (int *) shmat(shmid_matrix, NULL, 0);

    int shmid_partial_sum = shmget(IPC_PRIVATE, num_processes * sizeof(long), IPC_CREAT | 0666);
    long *partial_sum = (long *) shmat(shmid_partial_sum, NULL, 0);


    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
     * 
     * Hint: Treat starting address of shared memory region as first
     * elemnt of a 2D matrix i.e.,  matrix[0][0]
    */

    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i*COLUMN_COUNT+j] = i+j;
        }
    }


    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork). Note that each child process runs a new binary 
     * (i.e., binary of child_proc.c) using execlp()
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to store its
     * calculated partial sum.
     *
     *
     * Hints:
     *
     * Google "man fork" and "man execlp"
     *
    */

    int rows_per_process = ROWS_COUNT / num_processes;
    int remainder = ROWS_COUNT % num_processes;
    int start_row = 0;
    for (int p = 0; p < num_processes; p++) {
        int end_row = start_row + rows_per_process - 1;
        if (p == num_processes - 1) {
            end_row += remainder;
        }
        pid_t pid = fork();
        if (pid == 0) {
            char s_start[16], s_end[16], s_shmid_matrix[16], s_shmid_partial[16], s_index[16];
            sprintf(s_start, "%d", start_row);
            sprintf(s_end, "%d", end_row);
            sprintf(s_shmid_matrix, "%d", shmid_matrix);
            sprintf(s_shmid_partial, "%d", shmid_partial_sum);
            sprintf(s_index, "%d", p);
            execlp("./child", "./child", s_start, s_end, s_shmid_matrix, s_shmid_partial, s_index, NULL);
            perror("execlp");
            exit(1);
        }
        start_row = end_row + 1;
    }


    /* ToDo5
     * Wait for all children processes to complete
    */

    for (int p = 0; p < num_processes; p++) {
        wait(NULL);
    }



    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children processes)
     * and print the sum.
    */

    long total_sum = 0;
    for (int p = 0; p < num_processes; p++) {
        total_sum += partial_sum[p];
    }
    printf("Total Sum: %ld\n", total_sum);


    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */

    shmdt(matrix);
    shmdt(partial_sum);
    shmctl(shmid_matrix, IPC_RMID, NULL);
    shmctl(shmid_partial_sum, IPC_RMID, NULL);



    return 0;
}
