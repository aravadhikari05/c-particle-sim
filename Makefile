#adapted from jdah
CC = clang

CFLAGS = -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Ilib/glad/include -Ilib/glfw/include -fbracket-depth=1024
LDFLAGS = lib/glad/src/gl.o lib/glfw/src/libglfw3.a 

#required on macos
LDFLAGS += -framework QuartzCore -framework OpenGL -framework IOKit -framework Cocoa


SRC  = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: dirs libs game

libs:
	cd lib/glad && $(CC) -o src/gl.o -Iinclude -c src/gl.c
	cd lib/glfw && cmake . && make

dirs:
	mkdir -p ./$(BIN)

run: game
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: src/main_test
	./src/main_test

src/main_test: src/main_test.c src/List.c src/QuadTree.c
	$(CC) -o src/main_test src/main_test.c src/List.c src/QuadTree.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
