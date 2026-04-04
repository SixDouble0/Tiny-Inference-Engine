#include <stdlib.h>
#include <stdio.h>

#include "gemm.h"
void gemm_naive(
    const int8_t *A,  // weight of the matrix
    const int8_t *B,  // input matrix
    int32_t      *C,  // result 
    int           M,  // rows of A
    int           N,  // columns of B
    int           K   // columns of A / rows of B
) {
    // Naive implementation of matrix multiplication
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int32_t sum = 0;
            for (int k = 0; k < K; k++) {
                sum += (int32_t)A[i * K + k] * (int32_t)B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Performance test for the optimized GEMM implementation with O2 optimization
//Time taken for gemm naive: 176209 microseconds
//Time taken for TILE_SIZE 4: 251326 microseconds
//Time taken for TILE_SIZE 8: 217301 microseconds
//Time taken for TILE_SIZE 16: 202783 microseconds
//Time taken for TILE_SIZE 24: 198246 microseconds
//Time taken for TILE_SIZE 32: 196026 microseconds
//Time taken for TILE_SIZE 64: 191708 microseconds
//time mesurements may vary based on microcontroller you use.
//In general, the optimized version should be faster than the naive version, especially for larger matrix sizes,
//due to better cache utilization.
//but in my case because of low sram memory of ESP32, the performance improvement is not significant 
//for small matrix sizes like 100x100, but it becomes more evident as the matrix size increases.
//For ESP32 devkitv1 the best idea is to leave Naive GEMM as it is.
#define TILE_SIZE 64

void gemm_optimized(
    const int8_t *A,  // weight of the matrix
    const int8_t *B,  // input matrix
    int32_t      *C,  // result 
    int           M,  // rows of A
    int           N,  // columns of B
    int           K   // columns of A / rows of B
) {
    // Optimized implementation using tiling for better cache performance
    for (int i = 0; i < M; i += TILE_SIZE) {
        for (int j = 0; j < N; j += TILE_SIZE) {
            for (int k = 0; k < K; k += TILE_SIZE) {
                // Compute the tile C[i:i+TILE_SIZE, j:j+TILE_SIZE]
                for (int ii = i; ii < i + TILE_SIZE && ii < M; ii++) {
                    for (int jj = j; jj < j + TILE_SIZE && jj < N; jj++) {
                        int32_t sum = 0;
                        for (int kk = k; kk < k + TILE_SIZE && kk < K; kk++) {
                            sum += (int32_t)A[ii * K + kk] * (int32_t)B[kk * N + jj];
                        }
                        C[ii * N + jj] += sum;
                    }
                }
            }
        }
    }
}