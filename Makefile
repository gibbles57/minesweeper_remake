#Nicholas Gibbs ngibbs4 Project 2
CC = gcc
CFLAGS = -Wall -pedantic-errors -std=c99
TARGET = p2_ngibbs4_001
all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)
clean:
	rm -f $(TARGET)
