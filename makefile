CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Isrc -Itests 

test_arena: src/memory/arena.c tests/unity/unity.c tests/test_arena.c
	$(CC) $(CFLAGS) src/memory/arena.c tests/unity/unity.c tests/test_arena.c -o test_arena.exe

test_gemm: src/ops/gemm.c tests/unity/unity.c tests/test_gemm.c
	$(CC) $(CFLAGS) src/ops/gemm.c tests/unity/unity.c tests/test_gemm.c -o test_gemm.exe
clean:
	del test_arena.exe