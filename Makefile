CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myprog
SRC = notesearch.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)