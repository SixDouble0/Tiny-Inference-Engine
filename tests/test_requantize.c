#include "unity/unity.h"
#include "../src/ops/requantize.h"

void setUp(void) {}
void tearDown(void) {}

void requantize_clamp_test(void) {
    // Define input parameters for the requantization
    int size = 10;
    int32_t input[10] = {1000, -1000, 500, -500, 0, 2000, -2000, 1500, -1500, 2500};
    int8_t output[10] = {0};
    int32_t multiplier = 1; // No scaling
    int shift = 0; // No shift
    int32_t zero_point = 0; // Zero point for quantization

    // Expected output after requantization (clamped to int8 range)
    int8_t expected_output[10] = {127, -128, 127, -128, 0, 127, -128, 127, -128, 127};

    // Perform requantization
    requantize(input, output, size, multiplier, shift, zero_point);

    // Verify the output against the expected result
    for (int i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL_INT8(expected_output[i], output[i]);
    }
}

void requantize_scaling_test(void) {
    int32_t input[3] = {1000, 2000, 4000};
    int8_t output[3] = {0};
    int32_t multiplier = 1073741824; // 2^30
    int shift = 31;                  // >> 31
    int32_t zero_point = 0;

    // result = input * 2^30 >> 31 = input * 0.5
    // 1000 * 0.5 = 500 -> clamp -> 127
    // 2000 * 0.5 = 1000 -> clamp -> 127
    // 4000 * 0.5 = 2000 -> clamp -> 127
    int8_t expected[3] = {127, 127, 127};

    requantize(input, output, 3, multiplier, shift, zero_point);

    for (int i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL_INT8(expected[i], output[i]);
}    

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(requantize_clamp_test);
    RUN_TEST(requantize_scaling_test);
    return UNITY_END();
}