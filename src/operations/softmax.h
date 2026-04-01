#ifndef SOFTMAX_H
#define SOFTMAX_H

#include <math.h>
#include <stdint.h>

void softmax(const int32_t *data, int size, float *output);

#endif // SOFTMAX_H