#include <stdio.h>
#include "memory/arena.h"
#include "model/loader.h"
#include "../models/mnist_int8.h"

// Model architecture and dimensions
#define IN_H       28
#define IN_W       28
#define IN_CH       1

#define CONV1_OUT_CH  4
#define CONV1_OUT_H  26   // 28 - 3 + 1
#define CONV1_OUT_W  26

#define CONV2_OUT_CH  8
#define CONV2_OUT_H  24   // 26 - 3 + 1
#define CONV2_OUT_W  24

#define FC1_IN    (CONV2_OUT_H * CONV2_OUT_W * CONV2_OUT_CH)  // 4608
#define FC1_OUT   16
#define FC2_OUT   10

static int argmax(const float *arr, int size) {
    int best = 0;
    for (int i = 1; i < size; i++)
        if (arr[i] > arr[best]) best = i;
    return best;
}


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"




void app_main(void) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    Arena arena;
    arena_init(&arena, 154 * 1024); // 154 KB

    // Layer parameters 
    Conv2DParams conv1_params = {
        .weights    = conv1_weight,
        .in_h       = IN_H, .in_w = IN_W, .in_ch = IN_CH,
        .out_ch     = CONV1_OUT_CH,
        .kernel_h   = 3, .kernel_w = 3,
        .stride     = 1, .padding = 0,
        .multiplier = 1 << 20,
        .shift      = 20,
        .zero_point = 0
    };

    Conv2DParams conv2_params = {
        .weights    = conv2_weight,
        .in_h       = CONV1_OUT_H, .in_w = CONV1_OUT_W, .in_ch = CONV1_OUT_CH,
        .out_ch     = CONV2_OUT_CH,
        .kernel_h   = 3, .kernel_w = 3,
        .stride     = 1, .padding = 0,
        .multiplier = 1 << 20,
        .shift      = 20,
        .zero_point = 0
    };

    FlattenParams flat_params = {
        .size = FC1_IN
    };

    FCParams fc1_params = {
        .weights    = fc1_weight,
        .in_size    = FC1_IN,
        .out_size   = FC1_OUT,
        .multiplier = 1 << 20,
        .shift      = 20,
        .zero_point = 0
    };

    FCParams fc2_params = {
        .weights    = fc2_weight,
        .in_size    = FC1_OUT,
        .out_size   = FC2_OUT,
        .multiplier = 1 << 20,
        .shift      = 20,
        .zero_point = 0
    };

    // Model definition
    Layer layers[] = {
        { LAYER_TYPE_CONV2D,  &conv1_params },
        { LAYER_TYPE_RELU,    NULL          },
        { LAYER_TYPE_CONV2D,  &conv2_params },
        { LAYER_TYPE_RELU,    NULL          },
        { LAYER_TYPE_FLATTEN, &flat_params  },
        { LAYER_TYPE_FC,      &fc1_params   },
        { LAYER_TYPE_RELU,    NULL          },
        { LAYER_TYPE_FC,      &fc2_params   },
        { LAYER_TYPE_SOFTMAX, NULL          },
    };
    int num_layers = sizeof(layers) / sizeof(layers[0]);

    // Input - blank canvas (all zeros)
    int8_t input[IN_H * IN_W] = {0};

    // Inference
    float output[FC2_OUT];
    run_inference(layers, num_layers, input, output, &arena);

    // Result
    int digit = argmax(output, FC2_OUT);
    printf("Predicted digit: %d\n", digit);
    printf("Probabilities:\n");
    for (int i = 0; i < FC2_OUT; i++)
        printf("  %d: %.4f\n", i, output[i]);

    printf("Peak arena usage: %zu bytes\n", arena.peak_usage);

    arena_destroy(&arena);
}
