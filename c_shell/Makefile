CC = gcc
CFLAGS = -I./include -Wall
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = main

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ)
