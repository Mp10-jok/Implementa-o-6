CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -02
TARGET = banker

all: $(TARGET)

$(TARGET): banker.c
		$(CC) $(CFLAGS) -o $(TARGET) banker.c

clean:
		rm -f $(TARGET) *.o result.txt

.PHONY: all clean

all: banker

banker: banker.c
	gcc -Wall -Wextra -o banker banker.c

clean:
	rm -f banker result.txt