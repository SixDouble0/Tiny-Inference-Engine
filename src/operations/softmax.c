#include "softmax.h"

void softmax(const int32_t *data, int size, float *output) {

    // Sum of exponentials for normalization
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        output[i] = expf((float)data[i]);
        sum += output[i];
    }
    // Normalize the output
    for (int i = 0; i < size; i++) {
        output[i] /= sum;   
    }
}