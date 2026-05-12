CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = image_compressor
SRC = src/main.c src/bmp.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)