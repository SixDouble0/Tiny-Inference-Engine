# Tiny AI Inference Engine

Tiny AI Inference Engine is a compact, production-oriented neural network inference runtime written in C. The project focuses on deterministic memory usage, explicit data flow, and INT8-first execution for constrained environments.

This repository is centered on the core AI inference engine. ESP32 integration is available, validated, and fully usable for hardware tests and deployment, but it is optional.

## Why This Project

* Predictable memory behavior through a static arena allocator.
* INT8 execution path suitable for resource-constrained targets.
* Clear layer graph API that is easy to audit and extend.
* Minimal runtime complexity and no hidden allocations during inference.

## Core Capabilities

* **Quantized inference (INT8):** weights and activations are processed in 8-bit format.
* **Deterministic arena allocator:** memory is pre-allocated and tracked with peak usage.
* **Operator set in pure C:** `conv2d`, `gemm`, `im2col`, `relu`, `requantize`, `softmax`.
* **Layer-driven execution model:** a `Layer[]` graph is executed sequentially by the loader.

## Repository Layout

* `src/main.c` - reference application entry point.
* `src/model/` - graph execution (`run_inference`) and layer parameter contracts.
* `src/memory/` - static arena allocator.
* `src/operations/` - numerical kernels and post-processing operations.
* `models/` - exported quantized model weights (`mnist_int8.h`).
* `tests/` - Unity-based test suites for kernels and memory.

## Requirements

### Core Engine Development

* C toolchain with C11 support.
* Standard build environment for your platform.

### Optional ESP32 Workflow

* ESP32 board (for example `esp32doit-devkit-v1`).
* PlatformIO (VS Code extension or CLI).

## How To Use The Program

The default executable flow is defined in `src/main.c` and follows a standard inference pipeline.

### Runtime Sequence

1. The program initializes a static arena (`arena_init`) with a fixed memory budget.
2. Layer parameter structures are populated for Conv2D and Fully Connected layers.
3. The model graph is created as a `Layer[]` array.
4. Input tensor is prepared (`int8_t input[...]`).
5. `run_inference(...)` executes all layers in order.
6. Final probabilities are produced by Softmax into a float output buffer.
7. The predicted class is selected with `argmax` and printed.
8. Peak arena usage is reported, then the arena is destroyed.

## How `run_inference` Works

`run_inference` in `src/model/loader.c` executes the graph layer-by-layer:

1. **Conv2D**
    1. computes output shape,
    2. allocates temporary buffers from the arena,
    3. performs `im2col`,
    4. applies `gemm_naive`,
    5. requantizes back to INT8.
2. **ReLU**
    1. applies in-place activation on the current buffer.
3. **Flatten**
    1. updates logical tensor size without extra computation.
4. **Fully Connected**
    1. runs GEMM,
    2. requantizes output to INT8.
5. **Softmax**
    1. converts logits to float probabilities in the final output array.

## Hardware Execution On ESP32 (Optional)

Run firmware upload and monitor:

```bash
platformio run -t upload -t monitor -e esp32_run
```

Run all hardware tests:

```bash
platformio test -e esp32_test
```

Run one selected test suite:

```bash
platformio test -e esp32_test -f test_im2col -v
```

## Development Notes

* The core codebase is intentionally framework-light and explicit.
* ESP32 remains a supported and practical target for both validation and deployment.
