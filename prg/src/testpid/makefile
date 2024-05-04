# Compiler to use
CC := gcc

# Compiler flags, e.g., -Wall for all warnings
CFLAGS := -Wall

# Source directory for library files
SRC_DIR := ../../lib/pid

# Object files directory
OBJ_DIR := obj

# Target executable name
TARGET := pid_test

# Automatically list all .c files in SRC_DIR
SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Main file
MAIN_SRC := pid_test.c

# Object files to build
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Include directories
INCLUDES := -I$(SRC_DIR)

# Default make target
all: $(TARGET)

# Link object files into a binary
$(TARGET): $(OBJECTS) $(MAIN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(MAIN_SRC) $(OBJECTS)

# Compile source files into objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up objects and binary
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
