CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = image_compressor
SRC = src/main.c src/bmp.c src/processing.c src/metrics.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lm

clean:
	rm -f $(TARGET)