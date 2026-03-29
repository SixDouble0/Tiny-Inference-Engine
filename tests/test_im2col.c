#include "unity/unity.h"
#include "time.h"
#include "../src/ops/im2col.h"
#include "../src/ops/conv2d.h"
#include "../src/ops/gemm.h"

void setUp(void) {}
void tearDown(void) {}

void im2col_test(void) {
    // Define input parameters for the im2col transformation
    int in_h = 4, in_w = 4, in_ch = 1;
    int kernel_h = 2, kernel_w = 2;
    int stride = 1, padding = 0;

    // Define input feature map (4x4x1)
    int8_t input[16] = {
        1,  2,   3,  4,
        5,  6,   7,  8,
        9,  10,  11, 12,
        13, 14,  15, 16
    };

    // Calculate output dimensions for im2col transformation
    int out_h = (in_h + 2 * padding - kernel_h) / stride + 1;
    int out_w = (in_w + 2 * padding - kernel_w) / stride + 1;

    // Define output matrix after im2col transformation
    int8_t output[36] = {0};

    // Expected output after im2col transformation
    int8_t expected_output[36] = {
    1, 2, 5, 6,    // position (0,0)
    2, 3, 6, 7,    // position (0,1)
    3, 4, 7, 8,    // position (0,2)
    5, 6, 9, 10,   // position (1,0)
    6, 7, 10, 11,  // position (1,1)
    7, 8, 11, 12,  // position (1,2)
    9, 10, 13, 14, // position (2,0)
    10, 11, 14, 15,// position (2,1)
    11, 12, 15, 16 // position (2,2)
};

    // Perform im2col transformation
    im2col(input, output,
           in_h, in_w,
           in_ch,
           kernel_h, kernel_w,
           stride, padding);



    // Verify the output against the expected result
    for (int i = 0; i < out_h * out_w * kernel_h * kernel_w * in_ch; i++) {
        TEST_ASSERT_EQUAL_INT8(expected_output[i], output[i]);
    }
}

void im2col_vs_conv2d_test(void) {
    int in_h = 4, in_w = 4, in_ch = 1;
    int kernel_h = 2, kernel_w = 2;
    int stride = 1, padding = 0;
    int out_ch = 1;

    int8_t input[16] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16
    };

    int8_t weights[4] = { 1, 0, 0, 1 };  // filter 2x2

    //naive conv2d
    int32_t output_naive[9] = {0};
    conv2d(input, weights, output_naive,
           in_h, in_w, in_ch, out_ch,
           kernel_h, kernel_w, stride, padding);

    // im2col + gemm
    int8_t col[36] = {0};
    int32_t output_fast[9] = {0};


    im2col(input, col, in_h, in_w, in_ch, kernel_h, kernel_w, stride, padding);

    //             A[9×4]  B[4×1]   C[9×1]       M=9  N=1     K=4
    gemm_optimized(col,    weights, output_fast, 9,   out_ch, kernel_h * kernel_w * in_ch);


    // Comparison
    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_EQUAL_INT32(output_naive[i], output_fast[i]);
    }
}

void benchmark_im2col_vs_conv2d(void) {
    // bigger input to showcase a difference in performance, e.g. 28x28 like MNIST
    // np. 28x28 like MNIST
    static int8_t input[784];
    static int8_t weights[288]; // 32 filters 3x3x1
    static int32_t output_naive[26*26*32];
    static int8_t col[26*26*9];
    static int32_t output_fast[26*26*32];

    clock_t start, end; 

    // Initialize input and weights with some values
    for (int i = 0; i < 784; i++) input[i] = (int8_t)(i % 128);
    for (int i = 0; i < 288; i++) weights[i] = (int8_t)(i % 64);

    // naive conv2d
    start = clock();
    for (int i = 0; i < 1000; i++)
        conv2d(input, weights, output_naive, 28, 28, 1, 32, 3, 3, 1, 0);
    end = clock();
    printf("conv2d naive: %f ms\n", (double)(end-start)/CLOCKS_PER_SEC*1000/1000);

    // im2col + gemm
    start = clock();
    for (int i = 0; i < 1000; i++) {
    im2col(input, col, 28, 28, 1, 3, 3, 1, 0);
    gemm_optimized(col, weights, output_fast, 26*26, 32, 9);
    }
    end = clock();
    printf("im2col+gemm:  %f ms\n", (double)(end-start)/CLOCKS_PER_SEC*1000/1000);
}
int main(){
    UNITY_BEGIN();
    RUN_TEST(im2col_test);
    RUN_TEST(im2col_vs_conv2d_test);
    RUN_TEST(benchmark_im2col_vs_conv2d);
    return UNITY_END();
}

