#include "unity/unity.h"
#include "../src/ops/im2col.h"
#include "../src/ops/conv2d.h"

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

int main(){
    UNITY_BEGIN();
    RUN_TEST(im2col_test);
    return UNITY_END();
}