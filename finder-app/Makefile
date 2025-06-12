CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Werror
TARGET = writer

all: $(TARGET)

$(TARGET): writer.c
	$(CC) $(CFLAGS) writer.c -o writer

clean:
	rm -f $(TARGET) *.o
