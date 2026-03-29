CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Isrc -Itests 

test_arena: src/memory/arena.c tests/unity/unity.c tests/test_arena.c
	$(CC) $(CFLAGS) src/memory/arena.c tests/unity/unity.c tests/test_arena.c -o test_arena.exe

test_gemm: src/ops/gemm.c tests/unity/unity.c tests/test_gemm.c
	$(CC) $(CFLAGS) src/ops/gemm.c tests/unity/unity.c tests/test_gemm.c -o test_gemm.exe

test_conv2d: src/ops/conv2d.c tests/unity/unity.c tests/test_conv2d.c
	$(CC) $(CFLAGS) src/ops/conv2d.c tests/unity/unity.c tests/test_conv2d.c -o test_conv2d.exe

test_im2col: src/ops/im2col.c tests/unity/unity.c tests/test_im2col.c
	$(CC) $(CFLAGS) src/ops/im2col.c tests/unity/unity.c tests/test_im2col.c -o test_im2col.exe
clean:
	del test_arena.exe
	del test_gemm.exe
	del test_conv2d.exe