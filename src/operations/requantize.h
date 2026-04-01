#ifndef REQUANTIZE_H
#define REQUANTIZE_H

#include <stdint.h>

void requantize(
    const int32_t *input,      // input data to be requantized
    int8_t        *output,     // output buffer for requantized data
    int            size,       // number of elements in the input and output arrays
    int32_t        multiplier, // multiplier for scaling the input values
    int            shift,      // shift for scaling the input values (right shift)
    int32_t        zero_point  // zero point for quantization (int and float has different zero point, 
                               //so we need to pass it as a parameter)
);

#endif // REQUANTIZE_H