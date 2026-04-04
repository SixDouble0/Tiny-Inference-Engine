# Tiny Inference Engine (ESP32 Edition)

A lightweight, pure-C neural network inference engine built from scratch. This branch (esp32) is heavily optimized for embedded devices and microcontrollers with strict SRAM constraints, specifically the ESP32.

## Key Features

* **INT8 Quantization:** Neural network weights and activations are quantized to 8-bit integers, drastically reducing memory footprint and speeding up calculations on microcontrollers lacking hardware floating-point acceleration.
* **Static Memory Arena:** Utilizes a deterministic, pre-allocated memory arena to prevent dynamic heap fragmentation and avoid DRAM segment data does not fit compiler errors. Peak memory usage is tightly controlled (e.g., 150 KB maximum for a typical CNN).
* **Optimized Mathematical Operations:** Implements context-aware computing strategies. Extensive hardware benchmarking revealed that for small matrix sizes natively found on embedded systems, a naive approach combined with im2col data mapping significantly outperforms the looping overhead inherent to CPU-centric tiled algorithms.
* **Zero Dependencies:** Pure C implementation without standard C++ library overhead. Integrates natively with FreeRTOS and ESP-IDF via PlatformIO.
* **Hardware Unit Testing:** Comprehensive testing suite running natively on the ESP32 hardware using the Unity framework.

## Project Structure

* src/main.c - Main application entry point running the inference.
* src/operations/ - Core neural network algebra operations (gemm, conv2d, im2col, elu, softmax, equantize).
* src/model/ - The inference loader responsible for executing the layer-by-layer computational graph.
* src/memory/ - The deterministic rena memory allocator.
* models/ - Header files containing exported, INT8 quantized neural network weights (e.g., mnist_int8.h).
* 	ests/ - Segregated Unity test suites targeting individual mathematical operations on-device.

## Requirements

* **Hardware:** Any standard ESP32 development board (e.g., esp32doit-devkit-v1).
* **Software:** VS Code with the PlatformIO extension installed and configured.

## Code Example

The following snippet demonstrates how a developer can instantiate the engine, define a computational graph, and run inference using a minimal memory footprint.

`c
#include "memory/arena.h"
#include "model/loader.h"

void app_main(void) {
    // 1. Initialize deterministic memory arena (150 KB allocation)
    Arena arena;
    arena_init(&arena, 150 * 1024);

    // 2. Define layer parameters (referencing pre-loaded INT8 weights)
    Conv2DParams conv_params = {
        .weights = conv1_weight,
        .in_h = 28, .in_w = 28, .in_ch = 1,
        .out_ch = 4, .kernel_h = 3, .kernel_w = 3,
        .stride = 1, .padding = 0,
        .multiplier = 1048576, .shift = 20, .zero_point = 0
    };

    // 3. Assemble the computational graph
    Layer layers[] = {
        { LAYER_TYPE_CONV2D,  &conv_params },
        { LAYER_TYPE_RELU,    NULL },
        // ... (Additional layers here)
        { LAYER_TYPE_SOFTMAX, NULL }
    };
    int num_layers = sizeof(layers) / sizeof(layers[0]);

    // 4. Prepare input data and output buffer
    int8_t input_matrix[28 * 28] = { /* Input image data */ };
    float output_probabilities[10];

    // 5. Execute hardware inference
    run_inference(layers, num_layers, input_matrix, output_probabilities, &arena);

    // 6. Free the static arena
    arena_destroy(&arena);
}
`

## Build and Execution Guidelines

### Main Inference Deployment
To compile the engine, flash the firmware to the ESP32 via USB, and open the serial monitor to view classification outputs:

`ash
platformio run -t upload -t monitor -e esp32_run
`
*Note: A 2000ms TaskDelay is purposefully included at the beginning of pp_main to prevent serial logging loss while the USB bridge initializes.*

### Hardware Unit Tests
Execute the entire test suite natively on the ESP32 chip (validating mathematical consistency, benchmarking, and memory constraints via Unity):

`ash
platformio test -e esp32_test
`

To run a specific component test suite (e.g., im2col benchmarking):

`ash
platformio test -e esp32_test -f test_im2col -v
`

## Repository Architecture

* **esp32 branch** *(Current)*: Dedicated to ESP-IDF and microcontroller deployments. Replaces generic dynamic allocation with static memory arenas and utilizes PlatformIO tooling.
* **main branch**: The original POSIX/Desktop implementation using standard CMake, Makefiles, and unconstrained dynamic memory.
