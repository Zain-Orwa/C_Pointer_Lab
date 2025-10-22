CC=clang
CFLAGS=-Wall -Wextra -pedantic -O0 -g
ASAN_FLAGS=-fsanitize=address -fno-omit-frame-pointer
CH ?= 01_intro
BIN := $(CH)/main

.PHONY: all build run asan lldb clean
all: run
build:
	$(CC) $(CFLAGS) $(CH)/main.c -o $(BIN)
run: build
	./$(BIN)
asan:
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(CH)/main.c -o $(BIN)
	./$(BIN)
lldb: build
	lldb ./$(BIN)
clean:
	find . -name main -type f -delete
