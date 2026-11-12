CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC

TARGET = libmemoire.so

all: $(TARGET)

$(TARGET): libmemoire.c libmemoire.h
	$(CC) $(CFLAGS) -o libmemoire.o -c libmemoire.c
	$(CC) -shared -o $@ libmemoire.o -pthread

clean:
	rm -f *.o *.so
