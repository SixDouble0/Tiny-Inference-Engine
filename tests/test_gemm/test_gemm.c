#include <time.h>
#include <string.h>
#include <stdio.h>
#include "esp_timer.h" // ESP32 high-resolution timer
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <unity.h>
#include "../../src/operations/gemm.h"

// Define input matrices A and B, and the expected output matrix C
static int8_t A[4] = {1, 2, 3, 4}; // 2x2 matrix
static int8_t B[4] = {5, 6, 7, 8}; // 2x2 matrix
static int32_t C[4] = {0}; // Result matrix initialized to zero
static int32_t expected_C[4] = {19, 22, 43, 50}; // Expected result of A * B

void setUp(void) {
    memset(C, 0, sizeof(C));
}
void tearDown(void) {}


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
// Performance test comparing Naive and Tiled GEMM implementations
// NOT A UNITY TEST
void performance_test_gemm_comparison(void) {
    // Define larger input matrices A and B, and the output matrix C
    int M = 100;
    int N = 100;
    int K = 100;
    static int8_t A[10000]; // 100x100 matrix
    static int8_t B[10000]; // 100x100 matrix
    static int32_t C[10000] = {0}; // Result matrix initialized to zero

    // Initialize A and B with some values
    for (int i = 0; i < M * K; i++) {
        A[i] = (int8_t)(i % 128); // Fill A with values from 0 to 127
    }
    for (int i = 0; i < K * N; i++) {
        B[i] = (int8_t)((i + 1) % 128); // Fill B with values from 1 to 128
    }

    // Measure the time taken by the GEMM function
    
    // Naive GEMM performance test
    int64_t start_time_naive = esp_timer_get_time();
    gemm_naive(A, B, C, M, N, K);
    int64_t end_time_naive = esp_timer_get_time();

    printf("Time taken for gemm: %lld microseconds\n", end_time_naive - start_time_naive);

    // Test various tile sizes dynamically
    int tiles_to_test[] = {4, 8, 16, 24, 32, 64, 96, 128, 256, 512}; // Different tile sizes to test
    int num_tiles = sizeof(tiles_to_test) / sizeof(tiles_to_test[0]);

    for (int t = 0; t < num_tiles; t++) {
        int tile_size = tiles_to_test[t];
        memset(C, 0, sizeof(C)); // reset matrix for fair comparison
        
        int64_t start_time_tiled = esp_timer_get_time();
        gemm_dynamic_tile(A, B, C, M, N, K, tile_size);
        int64_t end_time_tiled = esp_timer_get_time();

        printf("Time taken for TILE_SIZE %d: %lld microseconds\n", tile_size, end_time_tiled - start_time_tiled);
    }
}



void app_main(void) {
    // Wait a second for serial port monitor to fully open
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    UNITY_BEGIN();
    RUN_TEST(test_gemm_naive_correctness);
    RUN_TEST(test_gemm_optimized_correctness);
    RUN_TEST(performance_test_gemm_comparison);
    UNITY_END();
}
