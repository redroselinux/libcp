CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = libcp
SRC = libcp.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	cp $(TARGET) /bin/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
