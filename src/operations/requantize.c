#include "requantize.h"

void requantize(
    const int32_t *input,      // input data to be requantized
    int8_t        *output,     // output buffer for requantized data
    int            size,       // number of elements in the input and output arrays
    int32_t        multiplier, // multiplier for scaling the input values
    int            shift,      // shift for scaling the input values (right shift)
    int32_t        zero_point  // zero point for quantization (int and float has different zero point, 
                               //so we need to pass it as a parameter)
){
    for (int i = 0; i < size; i++) {
        // multiplier for scaling the input values 
        int64_t result = (int64_t)input[i] * multiplier;                 
        // shift for scaling the input values (right shift)
        result >>= shift; 
        // Add zero point for quantization
        result += zero_point;

        // Clamp the output values to the int8 range [-128, 127]
        if (result > 127) {
            result = 127;
        } else if (result < -128) {
            result = -128;
        }
        // Store the requantized value in the output buffer
        output[i] = (int8_t)result;
    }
}