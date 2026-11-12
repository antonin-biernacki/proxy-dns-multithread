CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC

TARGET = libthread.so

all: $(TARGET)

$(TARGET): libthread.c libthread.h
	$(CC) $(CFLAGS) -o libthread.o -c libthread.c
	$(CC) -shared -o $@ libthread.o -pthread

clean:
	rm -f *.o *.so
