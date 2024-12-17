# **Makefile for Dungeon Adventure Game**

# Compiler to use
CC = gcc

# Compiler flags:
# -Wall: Enable all compiler's warning messages
# -Wextra: Enable additional warning flags
# -g: Generate debug information
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = dungeon_game

# Source files
SRCS = main.c game.c

# Object files (derived from source files)
OBJS = main.o game.o

# Header files
DEPS = game.h

# Default target executed when you run `make`
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile main.c into main.o
main.o: main.c $(DEPS)
	$(CC) $(CFLAGS) -c main.c

# Rule to compile game.c into game.o
game.o: game.c $(DEPS)
	$(CC) $(CFLAGS) -c game.c

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets are not files
.PHONY: all clean
