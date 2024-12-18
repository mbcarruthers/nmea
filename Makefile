CC = gcc
C_FLAGS = -Wall pedantic -std=c23 -O2
TARGET = main

C_SOURCES = main.c nmea.c sentences.c

DEFAULT_GOAL := help

.PHONY: clean run

help:
		@awk 'BEGIN {FS = ":.*##"; printf "\nAvailable commands:\n"} \
		/^[a-zA-Z_-]+:.*##/ { printf "  %-15s %s\n", $$1, $$2 }' $(MAKEFILE_LIST)\

all: $(TARGET) ## builds the whole project

$(TARGET): $(TARGET).o

main.o: main.c
nmea.o: nmea.c nmea.h sentences.h
sentences.o: sentences.c sentences.h


clean: ## clean the project files
	rm $(TARGET) $(TARGET).o sentences.o nmea.o main.o

run: ## run the program
	./$(TARGET)