#include <stdio.h>


#define N 8192
#define TILE 32
#define SIZE N*N


__global__ void transpose_gpu(double *b, const double *a, const int size)
{
    int x = blockIdx.x * TILE + threadIdx.x;
    int y = blockIdx.y * TILE + threadIdx.y;
    int width = gridDim.x * TILE;

    for (int i = 0; i < TILE; i+= size)
        b[x*width + (y + i)] = a[(y + i) * width + x];
}


__global__ void transpose_gpu_opt(double *b, const double *a, const int size)
{
    __shared__ double tmp[TILE][TILE];

    int x = blockIdx.x * TILE + threadIdx.x;
    int y = blockIdx.y * TILE + threadIdx.y;
    int id_a = x + (y) * N;

    x = blockIdx.y * TILE + threadIdx.x;
    y = blockIdx.x * TILE + threadIdx.y;
    int id_b = x + (y) * N;

    for (int i = 0; i < TILE; i+=size) {
        tmp[threadIdx.y + i][threadIdx.x] = a[id_a + i * N];
    }
    __syncthreads();

    for (int  i = 0; i < TILE; i+=size) {
        b[id_b + i * N] = tmp[threadIdx.x][threadIdx.y + i];
    }
}


void fill_matrix (double* a, const int dim) {
    for(int i = 0; i < dim; i++) {
        a[i] = (double) i;
    }
}


int main(int argc, char const *argv[]) {

    dim3 grid, block;
    block.x = TILE;
    block.y = atoi(argv[1]);
    grid.x = N / TILE;
    grid.y = N / TILE;

    double* host_input, * host_output;
    double* gpu_input, * gpu_output;

    host_input = (double* )malloc(SIZE * sizeof(double));
    host_output = (double* )malloc(SIZE * sizeof(double));

    cudaMalloc((void**)&gpu_input, SIZE * sizeof(double));
    cudaMalloc((void**)&gpu_output, SIZE * sizeof(double));

    char sep[] = "---------------------------------------------\n";
    printf("%sTHREADS x BLOCK: %d\n%s", sep, atoi(argv[1]), sep);


    //TRANSPOSE.
    fill_matrix(host_input, SIZE);

    cudaMemcpy(gpu_input, host_input, SIZE, cudaMemcpyHostToDevice);

    float total_time, total_time_opt = 0.0;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    transpose_gpu<<<grid,block>>>(gpu_input, gpu_output, block.y);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&total_time, start, stop);

    double bandwidth = 2. * (double)SIZE * sizeof(double) / total_time / 1e6;
    printf("TRANSPOSE:\t\t %fms\n", total_time);
    printf("BANDWIDTH:\t\t %fms\n%s", bandwidth, sep);

    cudaMemcpy(host_output, gpu_output, SIZE, cudaMemcpyDeviceToHost);


    // TRANSPOSE OPT.
    fill_matrix(host_input, SIZE);

    cudaMemcpy(gpu_input, host_input, SIZE, cudaMemcpyHostToDevice);

    cudaEventRecord(start,0);

    transpose_gpu_opt<<<grid, block>>>(gpu_input, gpu_output, block.y);

    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&total_time_opt, start, stop);

    double bandwidth_opt = 2. * (double)SIZE * sizeof(double) / total_time_opt / 1e6;
    printf("TRANSPOSE OPT:\t\t %fms\n", total_time_opt);
    printf("BANDWIDTH:\t\t %fms\n%s\n", bandwidth_opt, sep);

    cudaMemcpy(host_output, gpu_output, SIZE, cudaMemcpyDeviceToHost);


    free(host_input);
    free(host_output);
    cudaFree(gpu_input);
    cudaFree(gpu_output);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}
