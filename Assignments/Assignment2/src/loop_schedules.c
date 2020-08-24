#include <stdio.h>
#include <omp.h>


void print_usage( int * a, int N, int nthreads ) {

    int tid, i;
    for( tid = 0; tid < nthreads; ++tid ) {

        fprintf( stdout, "%d: ", tid );

        for( i = 0; i < N; ++i ) {

            if( a[ i ] == tid) fprintf( stdout, "*" );
            else fprintf( stdout, " ");
        }
        printf("\n");
    }
}


int main( int argc, char * argv[] ) {

    const int N = 40;
    int a[N];
    int thread_id = 0;
    int num_threads;

#pragma omp parallel
  {
    num_threads = omp_get_num_threads();
  };

    // SUMMARY.
    char *sep = "--------------------------------------------\n";
    printf("%sN: %i\t\t\t  THREADS: %i\n%s", sep, N, num_threads, sep);

    // SERIAL.
    for (int i = 0; i < N; ++i) {
        a[i] = thread_id;
    }

    printf("SERIAL\n%s", sep);
    print_usage(a, N, 1);

#pragma omp parallel
    {

// STATIC.
#pragma omp for schedule(static)
        for (int i = 0; i < N; ++i) {
            a[i] = omp_get_thread_num();
        }

#pragma omp single
        {
            printf("\n%sSTATIC\n%s", sep, sep);
            print_usage(a, N, num_threads);
        }

// STATIC, CHUNK: 1.
#pragma omp for schedule(static, 1)
        for (int i = 0; i < N; ++i) {
            a[i] = omp_get_thread_num();
        }

#pragma omp single
        {
            printf("\n%sSTATIC \t\t\t\t\t CHUNK SIZE: 1\n%s", sep, sep);
            print_usage(a, N, num_threads);
        }

// STATIC, CHUNK: 10.
#pragma omp for schedule(static, 10)
        for (int i = 0; i < N; ++i) {
            a[i] = omp_get_thread_num();
        }

#pragma omp single
        {
            printf("\n%sSTATIC \t\t\t\t\t CHUNK SIZE: 10\n%s", sep, sep);
            print_usage(a, N, num_threads);
        }

// DYNAMIC.
#pragma omp for schedule(dynamic)
        for (int i = 0; i < N; ++i) {
            a[i] = omp_get_thread_num();
        }

#pragma omp single
        {
            printf("\n%sDYNAMIC\n%s", sep, sep);
            print_usage(a, N, num_threads);
        }


// DYNAMIC, CHUNK: 1.
#pragma omp for schedule(dynamic, 1)
        for (int i = 0; i < N; ++i) {
            a[i] = omp_get_thread_num();
        }

#pragma omp single
        {
            printf("\n%sDYNAMIC \t\t\t\t\t CHUNK SIZE: 1\n%s", sep, sep);
            print_usage(a, N, num_threads);
        }


// DYNAMIC, CHUNK: 10.
#pragma omp for schedule(dynamic, 10)
        for (int i = 0; i < N; ++i) {
            a[i] = omp_get_thread_num();
        }

#pragma omp single
        {
            printf("\n%sDYNAMIC \t\t\t\t\t CHUNK SIZE: 10\n%s", sep, sep);
            print_usage(a, N, num_threads);
        }

    }

    return 0;
}
