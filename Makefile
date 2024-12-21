CC = gcc


SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
DATA_FILE = data/nmea.txt

DEBUG = -DDEBUG -g
C_SOURCES = $(shell find $(SRC_DIR) -type f -name "*.c")
C_INCLUDES = $(shell find $(INC_DIR) -type d | awk '{printf "-I%s \\\n", $$0}' | sed '$$ s/\\$$//')
O_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
C_FLAGS = -Wall -std=c23 -O2 $(C_INCLUDES)
#O_FILES = $(C_SOURCES:.c=.o)
DEFAULT_GOAL := help

.PHONY: help test all debug dall clean run

help:
		@awk 'BEGIN {FS = ":.*##"; printf "\nAvailable commands:\n"} \
		/^[a-zA-Z_-]+:.*##/ { printf "  %-15s %s\n", $$1, $$2 }' $(MAKEFILE_LIST)\

test: clean all run ## everything! all! at once!

all: $(TARGET) ## builds the whole project

$(TARGET): $(O_FILES) ## Link the target binary from object files
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) -o $@ $^

# Rule to compile each .c file into a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

debug: clean dall run ## clean, compile with debug, run

dall: ## compile with debug
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(DEBUG) $(C_SOURCES) -o $(TARGET)

clean: ## clean the project files
	rm -rf $(BUILD_DIR)

run: ## run the program
	./$(TARGET) $(DATA_FILE)

$(info Sources: $(C_SOURCES))
$(info Include Flags: $(C_INCLUDES))

