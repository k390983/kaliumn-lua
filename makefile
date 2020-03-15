CC = gcc

RM = rm

CFLAGS = -Wall
LFLAGS = -llua5.3
IFLAGS = -I/usr/include/lua5.3/

TARGET = kaliumn.c
OUTPUT = app

all:
	$(CC) $(CFLAGS) -o $(OUTPUT) $(TARGET) $(LFLAGS) $(IFLAGS)

clean:
	$(RM) $(OUTPUT)

