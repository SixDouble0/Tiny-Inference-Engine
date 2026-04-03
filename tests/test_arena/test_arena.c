#include <unity.h>
#include "../../src/memory/arena.h"

void setUp(void) {}
void tearDown(void) {}

// Test for arena initialization
void test_arena_init(void) {
    Arena arena = {0};
    arena_init(&arena, 1024);
    
    TEST_ASSERT_NOT_NULL(arena.memory); // Ensure memory was allocated
    TEST_ASSERT_EQUAL(1024, arena.size); // Check if the size is set correctly
    TEST_ASSERT_EQUAL(0, arena.used); // Initially, no memory should be used
    TEST_ASSERT_EQUAL(0, arena.peak_usage); // Peak usage should be 0 at initialization
    TEST_ASSERT_EQUAL(0, arena.global_peak_usage); // Global peak usage should also be 0 at initialization
    
    arena_destroy(&arena);
}

// Test for arena allocation
void test_arena_alloc(void) {
    Arena arena = {0};
    arena_init(&arena, 1024);
    
    void *ptr1 = arena_alloc(&arena, 256);
    TEST_ASSERT_NOT_NULL(ptr1); // Ensure allocation was successful
    TEST_ASSERT_EQUAL(256, arena.used); // Check if the used memory is updated correctly
    TEST_ASSERT_EQUAL(256, arena.peak_usage); // Peak usage should reflect the current allocation
    
    // Allocate more memory and check if the arena updates correctly
    void *ptr2 = arena_alloc(&arena, 512); 
    TEST_ASSERT_NOT_NULL(ptr2); 
    TEST_ASSERT_EQUAL(768, arena.used); 
    TEST_ASSERT_EQUAL(768, arena.peak_usage);
    
    
    arena_destroy(&arena);
}

// Test for arena allocation alignment
void test_arena_alignment(void) {
    Arena arena = {0};
    arena_init(&arena, 1024);
    
    arena_alloc(&arena, 13); //
    TEST_ASSERT_EQUAL(16, arena.used); // 13 bytes should be aligned to 16 bytes (next multiple of 8)
    
    arena_alloc(&arena, 1);
    TEST_ASSERT_EQUAL(24, arena.used); // 1 byte should be aligned to 8 bytes, so total used should be 16 (previous) + 8 (new allocation) = 24 bytes
    
    arena_destroy(&arena);
}

// Test for arena reset functionality
void test_arena_reset(void) {
    Arena arena = {0};
    arena_init(&arena, 1024);
    
    arena_alloc(&arena, 512); // Allocate some memory
    TEST_ASSERT_EQUAL(512, arena.used); // Check used memory before reset
    
    arena_reset(&arena); // Reset the arena
    TEST_ASSERT_EQUAL(0, arena.used); // After reset, used memory should be 0
    TEST_ASSERT_EQUAL(0, arena.peak_usage); // Peak usage should also be reset to 0
    
    arena_destroy(&arena);
}

void app_main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_arena_init);
    RUN_TEST(test_arena_alloc);
    RUN_TEST(test_arena_alignment);
    RUN_TEST(test_arena_reset);
    UNITY_END();
}

