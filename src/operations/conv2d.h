#ifndef CONV2D_H
#define CONV2D_H

#include <stdint.h>

void conv2d(
    const int8_t *input,    // input pixels 
    const int8_t *weights,  // filters - ready from the training phase
    int32_t      *output,   // result of convolution
    int in_h, int in_w,     // size of the input feature map
    int in_ch,              // number of input channels
    int out_ch,             // number of output channels (filters)
    int kernel_h, int kernel_w, // size of the filter
    int stride, int padding // stride and padding for convolution
);

#endif // conv2d.h