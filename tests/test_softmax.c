#include "unity/unity.h"
#include "../src/operations/softmax.h"

void setUp(void) {}
void tearDown(void) {}

void softmax_test(void) {
    int32_t input[4] = {1, 2, 3, 4};
    float output[4] = {0.0f};
    float expected[4] = {0.0321f, 0.0871f, 0.2369f, 0.6439f}; // Precomputed softmax values

    softmax(input, 4, output);
    
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected[i], output[i]);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(softmax_test);
    return UNITY_END();
}