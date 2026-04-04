#include "loader.h"
#include "../operations/gemm.h"
#include "../operations/im2col.h"
#include "../operations/requantize.h"
#include "../operations/relu.h"
#include "../operations/softmax.h"



int run_inference(const Layer *layers, int num_layers,
                  const int8_t *input, float *output,
                  Arena *arena) {

    const int8_t *current = input;
    int current_size = 0;

    for (int i = 0; i < num_layers; i++) {
        switch (layers[i].type) {

            case LAYER_TYPE_CONV2D: {
                Conv2DParams *ptr = (Conv2DParams *)layers[i].params;

                // Calculate output dimensions for convolution
                int out_h = (ptr->in_h + 2*ptr->padding - ptr->kernel_h) / ptr->stride + 1;
                int out_w = (ptr->in_w + 2*ptr->padding - ptr->kernel_w) / ptr->stride + 1;
                int col_size = out_h * out_w * ptr->kernel_h * ptr->kernel_w * ptr->in_ch;

                // allocate buffer for im2col output
                int8_t *col_buf = (int8_t *)arena_alloc(arena, col_size);

                // allocate buffer for gemm output (int32_t to avoid overflow before requantization)
                int32_t *gemm_out = (int32_t *)arena_alloc(arena, out_h * out_w * ptr->out_ch * sizeof(int32_t));

                // allocate buffer for convolution output (int8_t after requantization)
                int8_t *conv_out = (int8_t *)arena_alloc(arena, out_h * out_w * ptr->out_ch);

                im2col(current, col_buf, ptr->in_h, ptr->in_w, ptr->in_ch,
                       ptr->kernel_h, ptr->kernel_w, ptr->stride, ptr->padding);
                
                // Using optimized GEMM naive convolution because for small matrix sizes
                // it's performing the best on ESP32 devkitv1.
                gemm_naive(col_buf, ptr->weights, gemm_out,
                               out_h * out_w, ptr->out_ch, ptr->kernel_h * ptr->kernel_w * ptr->in_ch);
                
                requantize(gemm_out, conv_out, out_h * out_w * ptr->out_ch,
                           ptr->multiplier, ptr->shift, ptr->zero_point);
                
                current = conv_out;
                current_size = out_h * out_w * ptr->out_ch;
                break;
            }

            case LAYER_TYPE_RELU:
                relu((int8_t *)current, current_size); 
                break;

            case LAYER_TYPE_FLATTEN: {
                FlattenParams *ptr = (FlattenParams *)layers[i].params;
                // No computation needed - data layout stays the same
                // current_size updated for future models with pooling
                current_size = ptr->size;
                break;
            }
            case LAYER_TYPE_FC: {
                FCParams *ptr = (FCParams *)layers[i].params;

                // allocate buffer for gemm output (int32_t to avoid overflow before requantization)
                int32_t *gemm_out_fc = (int32_t *)arena_alloc(arena, ptr->out_size * sizeof(int32_t));

                // allocate buffer for fully connected layer output (int8_t after requantization)
                int8_t *fc_out = (int8_t *)arena_alloc(arena, ptr->out_size);

                gemm_optimized(current, ptr->weights, gemm_out_fc,
                               1, ptr->out_size, ptr->in_size);

                requantize(gemm_out_fc, fc_out, ptr->out_size,
                            ptr->multiplier, ptr->shift, ptr->zero_point);
                            
                current = fc_out; //
                current_size = ptr->out_size;
                break;
            }

            case LAYER_TYPE_SOFTMAX:
                softmax((int32_t *)current, current_size, output);
                break;
        }
    }
    return 0;
}