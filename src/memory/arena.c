#include <stdlib.h>
#include <stdio.h>

#include "arena.h"
// Note to me When defining a function that takes a pointer to a struct,
// do not forget to use * because & is available only in C++
void arena_init(Arena * arena, size_t size){
    arena->memory = malloc(size);
    // Check if malloc succeeded before proceeding
    if (arena->memory == NULL) {
        fprintf(stderr, "Failed to allocate memory for arena\n");
        exit(EXIT_FAILURE);
    }
    // Initialize arena metadata
    arena->size = size;
    arena->used = 0;
    arena->peak_usage = 0;
    arena->global_peak_usage = 0;
}

void *arena_alloc(Arena *arena, size_t size) {

    // Check if the arena memory is initialized before allocating
    if (arena->memory == NULL) {
        fprintf(stderr, "Arena memory is not initialized\n");
        exit(EXIT_FAILURE);
    }

    // Align size to 8 bytes 
    size_t aligned_size = (size + 7) & ~7;

    // Check if there is enough space in the arena for the requested allocation
    if (arena->used + aligned_size > arena->size) {
        fprintf(stderr, "Arena out of memory\n");
        exit(EXIT_FAILURE);
    }
    // Calculate the pointer to the allocated memory and update the used size
    void *ptr = arena->memory + arena->used;
    arena->used += aligned_size;

    // Update peak usage if the current used memory exceeds the previous peak
    if (arena->used > arena->peak_usage) {
        arena->peak_usage = arena->used;

    }
    // Update global peak usage if the current arena's peak usage exceeds the global peak   
    if (arena->peak_usage > arena->global_peak_usage) {
        arena->global_peak_usage = arena->peak_usage;
    }
    // Return the pointer to the allocated memory
    return ptr;
}

void arena_reset(Arena *arena){
    // Check if there is anything to reset
    if (arena->memory == NULL) {
        return;
    }

    arena->used = 0;
    // Print the peak usage for this inference before resetting it
    printf("Peak usage of this inference:  %zu KB\n", arena->peak_usage / 1024);
    arena->peak_usage = 0;
    
}

void arena_destroy(Arena *arena){
    // Check if there is any arena already existing
    if (arena->memory == NULL) {
        return;
    }
    // Free the allocated memory and reset the arena metadata
    free(arena->memory);
    arena->memory = NULL;
    arena->size = 0;
    arena->used = 0;
    arena->peak_usage = 0;
    // Print the peak of global usage of this instance before resetting it
    printf("Global peak usage of this instance: %zu KB\n", arena->global_peak_usage / 1024);
    arena->global_peak_usage = 0;
}