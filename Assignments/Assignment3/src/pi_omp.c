#include <stdio.h>
#include <omp.h>


double serial_pi(const double a, const double b, const unsigned int N) {

    const double h = (b-a)/(double)N;
    double x = a + h/2.;
    double pi = 0;

    for (int i = 0; i < N; i++) {
        pi += 4*h/(1.+ x*x);
        x += h;
    }
    return pi;
}

int main() {

    const unsigned int N = 1000000000;
    const int a = 0;
    const int b = 1;
    double pi = 0;
    int num_threads = omp_get_num_threads();

    // ATOMIC PI.
    double start = omp_get_wtime();

#pragma omp parallel default(none) shared(a, b, N, pi)
    {
        int tid = omp_get_thread_num();
        int threads = omp_get_num_threads();
        double h = (b - a) / (double)N;
        int local_n = (int) (N/threads);
        double local_a = a + tid*local_n*h;
        double local_b = local_a + local_n*h;
        double local_pi = serial_pi(local_a, local_b, (const unsigned int) local_n);

#pragma omp atomic
        pi += local_pi;
    }

    double total_time = omp_get_wtime() - start;


    // SUMMARY.
    char *sep = "-------------------------------------\n";
    printf("%sN: %i\t\t\t  THREADS: %i\n%s", sep, N, num_threads, sep);
    printf("\t\t\t\t\t  PI\t\t TIME \n");
    printf("OMP\t\t\t%.10f\t%fs\n", pi, total_time);
    printf("%s\n", sep);
    return 0;

}

