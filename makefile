
CC = gcc
LUA = lua5.3

RM = rm
CP = cp

CFLAGS = -Wall -shared -o
LFLAGS = -l$(LUA)
IFLAGS = -I/usr/include/$(LUA)/

TARGET = kaliumn
INCLUDES = src/kaliumn_lua.c src/includes/lodepng.c

default:
	@echo ""
	@echo "Please set target (lua, lua test, clean)"
	@echo ""

lua:
	@echo ""
	@echo "-------- COMPILE --------"
	@echo ""

	$(CC) src/$(TARGET).c $(INCLUDES) $(CFLAGS) $(TARGET).so -fPIC $(LFLAGS) $(IFLAGS)

	$(CP) $(TARGET).so tests

	@echo ""
	@echo "-------- END --------"
	@echo ""

lua-test:
	@echo ""
	@echo "-------- COMPILE --------"
	@echo ""

	$(CC) src/$(TARGET).c $(INCLUDES) $(CFLAGS) $(TARGET).so -fPIC $(LFLAGS) $(IFLAGS)

	$(CP) $(TARGET).so tests

	@echo ""
	@echo "-------- RUN --------"
	@echo ""

	cd tests; $(LUA) test.lua

	@echo ""
	@echo "-------- END --------"
	@echo ""

clean:
	@echo ""
	@echo "-------- CLEAN --------"
	@echo ""

	$(RM) $(TARGET).so
	$(RM) tests/$(TARGET).so

	@echo ""
	@echo "-------- END --------"
	@echo ""
