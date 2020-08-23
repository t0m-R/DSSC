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


double atomic_pi(const double a, const double b, const unsigned int N) {

  double pi = 0;

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

  return pi;
}


double critical_pi(const double a, const double b, const unsigned int N) {

  double pi = 0;

#pragma omp parallel default(none) shared(a, b, N, pi)
  {
    int tid = omp_get_thread_num();
    int threads = omp_get_num_threads();
    double h = (b - a) / (double)N;
    int local_n = (int) (N/threads);
    double local_a = a + tid*local_n*h;
    double local_b = local_a + local_n*h;
    double local_pi = serial_pi(local_a, local_b, (const unsigned int) local_n);

#pragma omp critical
        pi += local_pi;

  }

  return pi;
}


double reduction_pi(const double a, const double b, const unsigned int N) {

  double pi = 0;

#pragma omp parallel default(none) shared(a,b,N) reduction(+:pi)
  {
    int tid = omp_get_thread_num();
    int threads = omp_get_num_threads();
    double h = (b - a) / (double)N;
    int local_n = (int) (N/threads);
    double local_a = a + tid*local_n*h;
    double local_b = local_a + local_n*h;
      pi += serial_pi(local_a, local_b, (const unsigned int) local_n);

  }

  return pi;
}


int main() {

  const unsigned int N = 1000000000;
  const int a = 0;
  const int b = 1;
  double start;
  int threads = omp_get_num_threads();

  // SERIAL.
  start = omp_get_wtime();
  double serial = serial_pi(a, b, N);
  double serial_time = omp_get_wtime() - start;

  // ATOMIC.
  start = omp_get_wtime();
  double atomic = atomic_pi(a, b, N);
  double atomic_time = omp_get_wtime() - start;

  // CRITICAL.
  start = omp_get_wtime();
  double critical = critical_pi(a, b, N);
  double critical_time = omp_get_wtime() - start;

  // REDUCTION.
  start = omp_get_wtime();
  double reduction = reduction_pi(a, b, N);
  double reduction_time = omp_get_wtime() - start;

  // SUMMARY.
  char *sep = "-------------------------------------\n";
  printf("%sN: %i\t\t\t  THREADS: %i\n%s", sep, N, threads, sep);
  printf("\t\t\t\t\t  PI\t\t TIME \n");
  printf("SERIAL\t\t%.10f\t%fs\n", serial, serial_time);
  printf("ATOMIC\t\t%.10f\t%fs\n", atomic, atomic_time);
  printf("CRITICAL\t%.10f\t%fs\n", critical, critical_time);
  printf("REDUCTION\t%.10f\t%fs\n", reduction, reduction_time);
  printf("%s\n", sep);

  return 0;
}