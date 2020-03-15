CC = gcc

RM = rm

CFLAGS = -Wall
LFLAGS = -llua5.2
IFLAGS = -I/usr/include/lua5.2/

TARGET = kaliumn.c
OUTPUT = app

all:
	$(CC) $(CFLAGS) -o $(OUTPUT) $(TARGET) $(LFLAGS) $(IFLAGS)

clean:
	$(RM) $(OUTPUT)

