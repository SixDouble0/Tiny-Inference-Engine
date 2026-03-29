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


//Time taken for size 32: 1.458000 seconds
#define TILE_SIZE 16

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