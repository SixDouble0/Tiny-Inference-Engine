#ifndef LOADER_H
#define LOADER_H
#include "../memory/arena.h"

#include <stdint.h>

// Enum for different layer types
typedef enum {
    LAYER_TYPE_CONV2D,
    LAYER_TYPE_RELU,
    LAYER_TYPE_FLATTEN,
    LAYER_TYPE_FC,
    LAYER_TYPE_SOFTMAX
} LayerType;

typedef struct {
    LayerType type;
    void *params;
} Layer;

typedef struct {
    const int8_t *weights;  // filter weights
    int in_h, in_w;         // input size
    int in_ch;              // input channels
    int out_ch;             // output channels
    int kernel_h, kernel_w; // filter size
    int stride, padding;  
    int32_t multiplier; 
    int shift;  
    int32_t zero_point;     // requantization zero point
} Conv2DParams; // Convolutional layer parameters

typedef struct {
    const int8_t *weights;  // fully connected layer weights
    int in_size;            // input size
    int out_size;           // output size
    int32_t multiplier;     // requantization multiplier
    int shift;              // requantization shift
    int32_t zero_point;     // requantization zero point
} FCParams; // Fully Connected layer parameters

typedef struct {
    int size;  
} FlattenParams; // Flatten layer parameters (currently only size, but can be extended for pooling)

int run_inference(const Layer *layers, int num_layers, 
                  const int8_t *input, float *output,
                  Arena *arena);

#endif