#include "softmax.h"

void softmax(const int8_t *data, int size, float *output) {
    // Calculate the max element to prevent expf() overflow
    int8_t max_val = data[0];
    for (int i = 1; i < size; i++) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    // Sum of exponentials for normalization
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        output[i] = expf((float)(data[i] - max_val));
        sum += output[i];
    }
    // Normalize the output
    for (int i = 0; i < size; i++) {
        output[i] /= sum;   
    }
}