#include "im2col.h"

void im2col(const int8_t *input,    // input feature map
    int8_t       *output,   // output matrix after im2col transformation
    int in_h, int in_w,     // size of the input feature map
    int in_ch,              // number of input channels
    int kernel_h, int kernel_w, // size of the convolution kernel
    int stride, int padding // stride and padding for convolution
){
    // Calculate output dimensions
    int out_h = (in_h + 2 * padding - kernel_h) / stride + 1;
    int out_w = (in_w + 2 * padding - kernel_w) / stride + 1;

    //iterating over the output
    for (int i = 0; i < out_h; i++){
        for (int j = 0; j < out_w; j++){
            //iterating over the kernel 
            for (int k = 0; k < kernel_h; k++){
                for (int l = 0; l < kernel_w; l++){
                    //iterating over the input channels
                    for (int c = 0; c < in_ch; c++){
                        // limits of the input pixel position, accounting for stride and padding
                        int in_row = i * stride + k - padding;
                        int in_col = j * stride + l - padding;
                        int8_t val = 0; // Handle padding by treating out-of-bounds input as zero
                        if (in_row >= 0 && in_row < in_h &&
                            in_col >= 0 && in_col < in_w) {
                            val = input[in_row * in_w * in_ch + in_col * in_ch + c];
                        }
                        // Calculate the output index for the matrix after im2col transformation
                        int row = i * out_w + j;
                        int col = k * kernel_w * in_ch + l * in_ch + c;
                        int out_idx = row * (kernel_h * kernel_w * in_ch) + col;
                        output[out_idx] = val; // Store the input pixel value in the output matrix
                    }
                }
            }
        }
    }
}