CC = gcc
C_FLAGS = -Wall -std=c23 -O2
TARGET = main

 ## Todo Sort and actually make the Makefile Robust, McCain
C_SOURCES = main.c cbuf.c nmea.c sentences.c
O_FILES = $(C_SOURCES:.c=.o)
DEFAULT_GOAL := help

.PHONY: clean run

help:
		@awk 'BEGIN {FS = ":.*##"; printf "\nAvailable commands:\n"} \
		/^[a-zA-Z_-]+:.*##/ { printf "  %-15s %s\n", $$1, $$2 }' $(MAKEFILE_LIST)\

test: clean all run ## everything! all! at once!

all: $(TARGET) ## builds the whole project

# Link the target binary from object files
$(TARGET): $(O_FILES)
	$(CC) $(C_FLAGS) -o $@ $^

# Compile each .c file into a .o file
%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@

clean: ## clean the project files
	rm -f $(TARGET) $(O_FILES)


run: ## run the program
	./$(TARGET)


