
CC = gcc
LUA = lua5.3

RM = rm

CFLAGS = -Wall -shared -o
LFLAGS = -l$(LUA)
IFLAGS = -I/usr/include/$(LUA)/

TARGET = kaliumn

default:
	@echo ""
	@echo "-------- COMPILE --------"
	@echo ""
	$(CC) $(TARGET).c $(CFLAGS) $(TARGET).so -fPIC $(LFLAGS) $(IFLAGS)
	@echo ""
	@echo "-------- END --------"
	@echo ""

run:
	@echo ""
	@echo "-------- COMPILE --------"
	@echo ""

	$(CC) $(TARGET).c $(CFLAGS) $(TARGET).so -fPIC $(LFLAGS) $(IFLAGS)
	
	@echo ""
	@echo "-------- RUN --------"
	@echo ""

	$(LUA) test.lua

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
