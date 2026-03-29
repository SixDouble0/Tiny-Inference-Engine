#ifndef GEMM_H
#define GEMM_H

#include <stddef.h>
#include <stdint.h>


void gemm_naive(
    const int8_t *A,  // weight of the matrix
    const int8_t *B,  // input matrix
    int32_t      *C,  // result 
    int           M,  // rows of A
    int           N,  // columns of B
    int           K   // columns of A / rows of B
);

void gemm_optimized(
    const int8_t *A,  // weight of the matrix
    const int8_t *B,  // input matrix
    int32_t      *C,  // result 
    int           M,  // rows of A
    int           N,  // columns of B
    int           K   // columns of A / rows of B
);

#endif // gemm.h