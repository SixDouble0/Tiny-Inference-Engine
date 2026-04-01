#include "unity/unity.h"

#include "../src/operations/relu.h"

void setUp(void) {}
void tearDown(void) {}

void relu_test(void) {
    int8_t data[10] = {-5, -3, -1, 0, 1, 3, 5, -2, 2, -4};
    int8_t expected[10] = {0, 0, 0, 0, 1, 3, 5, 0, 2, 0};

    relu(data, 10);

    for (size_t i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT8(expected[i], data[i]);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(relu_test);
    return UNITY_END();
}