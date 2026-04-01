#include "conv2d.h"

void conv2d(
    const int8_t *input,        // input pixels
    const int8_t *weights,      // filters - ready from the training phase
    int32_t      *output,       // result of convolution
    int in_h, int in_w,         // size of the input feature map
    int in_ch,                  // number of input channels
    int out_ch,                 // number of output channels (filters)
    int kernel_h, int kernel_w, // size of the filter
    int stride, int padding     // stride and padding for convolution
) {
    // Calculate output dimensions
    int out_h = (in_h + 2 * padding - kernel_h) / stride + 1; 
    int out_w = (in_w + 2 * padding - kernel_w) / stride + 1; 

    // Perform convolution
   // Iterate over the output feature map 
    for (int i = 0; i < out_h; i++) {
        for (int j = 0; j < out_w; j++) {
            // Iterate over the output channels (filters)
            for (int f = 0; f < out_ch; f++) {
                int32_t sum = 0; // Accumulate the convolution sum for the current output pixel
                // Iterate over the kernel
                for (int k = 0; k < kernel_h; k++) {
                    for (int l = 0; l < kernel_w; l++) {
                        // Iterate over the input channels inside the kernel
                        for (int c = 0; c < in_ch; c++) {
                            // Calculate the corresponding input pixel position, accounting for stride and padding
                            int in_row = i * stride + k - padding;
                            int in_col = j * stride + l - padding;
                            int8_t val = 0; // Handle padding by treating out-of-bounds input as zero
                            if (in_row >= 0 && in_row < in_h &&
                                in_col >= 0 && in_col < in_w) {
                                val = input[in_row * in_w * in_ch + in_col * in_ch + c];
                            }
                            int w_idx = f * kernel_h * kernel_w * in_ch +
                                        k * kernel_w * in_ch +
                                        l * in_ch + c;
                            sum += (int32_t)val * (int32_t)weights[w_idx]; // Accumulate the convolution sum
                        }
                    }
                }
                output[i * out_w * out_ch + j * out_ch + f] = sum; // Store the computed convolution result in the output feature map
            }
        }
    }
}