CC = gcc

RM = rm

CFLAGS = -Wall -shared -o
LFLAGS = -llua5.3
IFLAGS = -I/usr/include/lua5.3/

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

	lua test.lua

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