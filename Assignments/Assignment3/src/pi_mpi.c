#include <stdio.h>
#include <mpi.h>


int main(int argc, char* argv[]) {

    const unsigned int N = 1000000000;
    const double h = 1./N;

    int rank = 0;
    int n_proc = 1;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    const int last = n_proc - 1; //print output
    const int root = 0;          //perform reduction

    const unsigned int local_N = N / n_proc;
    double local_pi = 0;
    const double start = rank * local_N;
    const double end = start + local_N;


    double start_time = MPI_Wtime();

    for (int i = (int) start; i < end; i++) {
        local_pi += 4.0 / (1.0 + (h * (i + 0.5)) * (h * (i + 0.5)));
    }
    local_pi *= h;

    double pi;
    MPI_Reduce(&local_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    double total_time = MPI_Wtime() - start_time;

    if (rank == root) {
        MPI_Send(&pi, 1, MPI_DOUBLE, last, 101, MPI_COMM_WORLD);
    }

    if (rank == last) {
        MPI_Recv(&pi, 1, MPI_DOUBLE, root, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // SUMMARY.
        char *sep = "-------------------------------------\n";
        printf("%sN: %i\t\t\t  THREADS: %i\n%s", sep, N, n_proc, sep);
        printf("\t\t\t\t\t  PI\t\t TIME \n");
        printf("MPI\t\t\t%.10f\t%fs\n", pi, total_time);
        printf("%s\n", sep);
    }

    MPI_Finalize();

    return 0;
}