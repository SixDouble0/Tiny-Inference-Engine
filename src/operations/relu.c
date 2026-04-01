#include "relu.h"

void relu(int8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (data[i] < 0) {
            data[i] = 0; // Set negative values to zero
        }
    }
}