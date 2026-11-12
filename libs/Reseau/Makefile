CC = gcc
CFLAGS = -Wall -Wextra -Werror

TARGET = ../static/libreseau.a

all: $(TARGET)

$(TARGET): libreseau.c libreseau.h dns_server.h
	$(CC) -c $(CFLAGS) -o libreseau.o $<
	ar rcs $@ libreseau.o

clean:
	rm -f *.o *.a
	rm -f ../static/*.a
