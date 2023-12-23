# CC=gcc
# CFLAGS=-c -Wall

# all: prog

# prog: drawables.o main.o
# 	$(CC) ./build/main.o ./build/drawables.o -lraylib -o ./build/rayhop_square

# drawables.o: lib/drawables/drawables.c
# 	mkdir -p ./build
# 	$(CC) $(CFLAGS) lib/drawables/drawables.c -o ./build/drawables.o

# main.o: ./build/drawables.o
# 	$(CC) $(CFLAGS) main.c -o ./build/main.o

# clean:
# 	rm -rf ./build/*.o

TARGET = build/rayhop_square
LIBS = -lraylib
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(shell find . -name "*.c"))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	rm -rf $(shell find . -name "*.o")