#include "unity/unity.h"
#include "../src/ops/conv2d.h"

void setUp(void) {}
void tearDown(void) {}

void conv2d_test(void) {
    // Define input parameters for the convolution
    int in_h = 5, in_w = 5, in_ch = 1;
    int out_ch = 1, kernel_h = 3, kernel_w = 3;
    int stride = 1, padding = 0;

    // Define input feature map (5x5x1)
    int8_t input[25] = {
        1, 2, 3, 0, 1,
        0, 1, 2, 3, 0,
        1, 0, 1, 2, 3,
        0, 1, 0, 1, 2,
        3, 0, 1, 0, 1
    };

    // Define convolutional filter (3x3x1)
    int8_t weights[9] = {
        1, 0, -1,
        1, 0, -1,
        1, 0, -1
    };

    // Calculate output dimensions
    int out_h = (in_h + 2 * padding - kernel_h) / stride + 1;
    int out_w = (in_w + 2 * padding - kernel_w) / stride + 1;

    // Define output feature map (3x3x1)
    int32_t output[9] = {0};

    // Expected output after convolution
    int32_t expected_output[9] = {
    -4, -2,  2,
    -2, -4, -2,
     2, -2, -4
    };

    // Perform convolution
    conv2d(input, weights, output,
           in_h, in_w,
           in_ch,
           out_ch,
           kernel_h, kernel_w,
           stride, padding);

       printf("out_h=%d out_w=%d\n", out_h, out_w);
    for (int i = 0; i < out_h * out_w * out_ch; i++) {
        printf("output[%d] = %d\n", i, output[i]);
    }
    // dopiero potem asserty
    for (int i = 0; i < out_h * out_w * out_ch; i++) {
        TEST_ASSERT_EQUAL_INT32(expected_output[i], output[i]);
    }
}
int main(){
    UNITY_BEGIN();
    RUN_TEST(conv2d_test);
    return UNITY_END();
}