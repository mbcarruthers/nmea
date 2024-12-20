CC = gcc
C_FLAGS = -Wall -std=c23 -O2
TARGET = main

DEBUG = -DDEBUG -g
 ## Todo Sort and actually make the Makefile Robust, McCain
C_SOURCES = main.c cbuf.c nmea.c sentences.c
O_FILES = $(C_SOURCES:.c=.o)
DEFAULT_GOAL := help

.PHONY: help test all debug dall clean run

help:
		@awk 'BEGIN {FS = ":.*##"; printf "\nAvailable commands:\n"} \
		/^[a-zA-Z_-]+:.*##/ { printf "  %-15s %s\n", $$1, $$2 }' $(MAKEFILE_LIST)\

test: clean all run ## everything! all! at once!

all: $(TARGET) ## builds the whole project


$(TARGET): $(O_FILES) ## Link the target binary from object files
	$(CC) $(C_FLAGS) -o $@ $^


%.o: %.c ## Compile each .c file into a .o file
	$(CC) $(C_FLAGS) -c $< -o $@

debug: clean dall run ## clean, compile with debug, run

dall: ## compile with debug
	$(CC) $(C_FLAGS) $(DEBUG) $(C_SOURCES) -o $(TARGET)

clean: ## clean the project files
	rm -f $(TARGET) $(O_FILES)

run: ## run the program
	./$(TARGET)


