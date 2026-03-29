#include <time.h>
#include <string.h>

#include "unity/unity.h"
#include "../src/ops/gemm.h"


void setUp(void) {
    memset(C, 0, sizeof(C));
}
void tearDown(void) {}

// Define input matrices A and B, and the expected output matrix C
static int8_t A[4] = {1, 2, 3, 4}; // 2x2 matrix
static int8_t B[4] = {5, 6, 7, 8}; // 2x2 matrix
static int32_t C[4] = {0}; // Result matrix initialized to zero
static int32_t expected_C[4] = {19, 22, 43, 50}; // Expected result of A * B


// Test for the naive GEMM 
void test_gemm_naive_correctness(void) {
    // Call the GEMM function
    gemm_naive(A, B, C, 2, 2, 2);
    // Verify that the output matches the expected result
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT32(expected_C[i], C[i]);
    }
}

// Test for the optimized GEMM 
void test_gemm_optimized_correctness(void) {
    // Call the optimized GEMM function
    gemm_optimized(A, B, C, 2, 2, 2);
    // Verify that the output matches the expected result
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT32(expected_C[i], C[i]);
    }
}
// Performance test for the naive GEMM implementation
// NOT A UNITY TEST
void performance_test_gemm_naive(void) {
    // Define larger input matrices A and B, and the output matrix C
    int M = 1000;
    int N = 1000;
    int K = 1000;
    static int8_t A[1000000]; // 1000x1000 matrix
    static int8_t B[1000000]; // 1000x1000 matrix
    static int32_t C[1000000] = {0}; // Result matrix initialized to zero

    // Initialize A and B with some values
    for (int i = 0; i < M * K; i++) {
        A[i] = (int8_t)(i % 128); // Fill A with values from 0 to 127
    }
    for (int i = 0; i < K * N; i++) {
        B[i] = (int8_t)((i + 1) % 128); // Fill B with values from 1 to 128
    }

    // Measure the time taken by the GEMM function

    //Naive GEMM performance test
    clock_t start_time_naive = clock();
    gemm_naive(A, B, C, M, N, K);
    clock_t end_time_naive = clock();

    //Optimized GEMM performance test
    clock_t start_time_optimized = clock();
    gemm_optimized(A, B, C, M, N, K);
    clock_t end_time_optimized = clock();

    // Calculate and print the time taken for both implementations
    double time_taken_naive = (double)(end_time_naive - start_time_naive) / CLOCKS_PER_SEC;
    double time_taken_optimized = (double)(end_time_optimized - start_time_optimized) / CLOCKS_PER_SEC;
    printf("Time taken for gemm_naive: %f seconds\n", time_taken_naive);
    printf("Time taken for gemm_optimized: %f seconds\n", time_taken_optimized);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_gemm_naive_correctness);
    RUN_TEST(test_gemm_optimized_correctness);
    RUN_TEST(performance_test_gemm_naive);
    return UNITY_END();
}