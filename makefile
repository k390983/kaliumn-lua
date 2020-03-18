
CC = gcc
LUA = lua5.3

RM = rm

CFLAGS = -Wall -shared -o
LFLAGS = -l$(LUA)
IFLAGS = -I/usr/include/$(LUA)/

TARGET = kaliumn
INCLUDES = src/includes/interface.c src/includes/canvas.c src/includes/etc.c

default:
	@echo ""
	@echo "-------- COMPILE --------"
	@echo ""
	$(CC) src/$(TARGET).c $(INCLUDES) $(CFLAGS) $(TARGET).so -fPIC $(LFLAGS) $(IFLAGS)
	@echo ""
	@echo "-------- END --------"
	@echo ""

test:
	@echo ""
	@echo "-------- COMPILE --------"
	@echo ""

	$(CC) src/$(TARGET).c $(INCLUDES) $(CFLAGS) $(TARGET).so -fPIC $(LFLAGS) $(IFLAGS)
	
	@echo ""
	@echo "-------- RUN --------"
	@echo ""

	$(LUA) tests/test.lua

	@echo ""
	@echo "-------- END --------"
	@echo ""

clean:
	@echo ""
	@echo "-------- CLEAN --------"
	@echo ""
	$(RM) $(TARGET).so
	@echo ""
	@echo "-------- END --------"
	@echo ""
