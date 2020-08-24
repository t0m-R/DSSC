#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


void print_matrix(int* M, int rows, int cols) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%d  ", M[i * cols + j]);
        }
        printf("\n");
    }
}


void print_matrix_file(int* M, int rows, int cols, FILE* file) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            fprintf(file, "%d  ", M[i * cols + j]);
        }
        fprintf(file, "\n");
    }
}


int main(int argc, char* argv[]) {

    const int N = atoi(argv[1]);
    int rank = 0;
    int n_proc = 1;
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    int local_N = N / n_proc;
    int modulus = N % n_proc;

    if (rank < modulus) {
        local_N += 1;
    }

    int* M = (int* )malloc(local_N * N * sizeof(int));

    int cor_size = 0;
    if (rank >= modulus && modulus != 0) cor_size = 1;

    for (size_t i = 0; i < local_N; i++) {
        for (size_t j = 0; j < N; j++) {
            if (j == i + rank * local_N + cor_size * modulus) {
                M[i * N + j] = 1;
            } else {
                M[i * N + j] = 0;
            }
        }
    }

    if (rank != root) {
        MPI_Send(M, local_N * N, MPI_INT, root, 101, MPI_COMM_WORLD);
    }
    else {
        if (N < 10) {

            // CONSOLE OUTPUT.
            print_matrix(M, local_N, N);
            for (int proc = 1; proc < n_proc; proc++) {
                if (proc >= modulus && modulus != 0) cor_size = 1;
                MPI_Recv(M, local_N * N, MPI_INT, proc, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_matrix(M, local_N - cor_size, N);
            }

        } else {

            // FILE OUTPUT.
            FILE* file = fopen("matrix.txt", "w");
            print_matrix_file(M, local_N, N, file);
            for (int proc = 1; proc < n_proc; proc++) {
                if (proc >= modulus && modulus != 0) cor_size = 1; // Correct printing in case of modulus.
                MPI_Recv(M, local_N * N, MPI_INT, proc, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_matrix_file(M, local_N - cor_size, N, file);
            }
            fclose(file);

        }

    }

    free(M);

    MPI_Finalize();

    return 0;
}