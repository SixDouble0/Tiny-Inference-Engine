#ifndef IM2COL_H
#define IM2COL_H

#include <stdint.h>


void im2col(
    const int8_t *input,    // input feature map
    int8_t       *output,   // output matrix after im2col transformation
    int in_h, int in_w,     // size of the input feature map
    int in_ch,              // number of input channels
    int kernel_h, int kernel_w, // size of the convolution kernel
    int stride, int padding // stride and padding for convolution
);


#endif // im2col.h