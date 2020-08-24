#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


#define SIZE 10000000
#define RECEIVER(rank, n_proc) (rank - 1 + n_proc) % n_proc
#define SENDER(rank, n_proc) (rank + 1 + n_proc) % n_proc


void swap(int** A, int** B) {
    int* Tmp = *A;
    *A = *B;
    *B = Tmp;
}

int main(int argc, char* argv[]) {

    int rank = 0;
    int n_proc = 1;
    int root = 0;
    int i,j,z;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    MPI_Request requestSend; // Request handler, checks communication status.
    MPI_Request requestRecv; // Request handler, checks communication status.

    int* send_buffer = (int*)malloc(sizeof(int) * SIZE);
    int* recv_buffer = (int*)malloc(sizeof(int) * SIZE);
    int* sum = (int*)malloc(sizeof(int) * SIZE);

    // Initialize send_buffer and sum.
    for( i = 0; i < SIZE; i++) {
        send_buffer[i] = rank;
        sum[i] = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // Cycle the whole chain.
    for ( j = 0; j < n_proc; j++) {

        // send_buffer -> RECEIVER
        MPI_Isend(send_buffer, SIZE, MPI_INT, RECEIVER(rank, n_proc), 101,
                  MPI_COMM_WORLD, &requestSend);
        // SENDER -> recv_buffer
        MPI_Irecv(recv_buffer, SIZE, MPI_INT, SENDER(rank, n_proc), 101,
                  MPI_COMM_WORLD, &requestRecv);

        // Update sum.
        for ( z = 0; z < SIZE; z++) {
            sum[z] += send_buffer[z];
        }

        MPI_Wait(&requestSend, MPI_STATUS_IGNORE);
        MPI_Wait(&requestRecv, MPI_STATUS_IGNORE);

        swap(&send_buffer, &recv_buffer);

    }

    MPI_Barrier(MPI_COMM_WORLD);
    double total_time = MPI_Wtime() - start;

    // SUMMARY.
    char *sep = "-------------------------------------\n";

    if (rank == root) {
        printf("%sN: %i\t\t\t  THREADS: %i\n", sep, SIZE, n_proc);
        printf("TIME: %f\n%s",total_time, sep);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    printf("PROC: %d\t\t\t\t SUM: %d\n", rank, sum[0]);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == root) {
        printf("%s\n", sep);
    }

    free(send_buffer);
    free(recv_buffer);
    free(sum);

    MPI_Finalize();

}
