CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c
TARGET = ../bin/main
LIBS = ../libs/static/libreseau.a ../libs/Memoire/libmemoire.so ../libs/Threads/libthread.so ../libs/Log/libgenericLog.so

all: $(TARGET)

# Relie a nouveau si une bibliotheque a change
$(TARGET): $(SRC) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $(SRC) -L../libs/static -lreseau -L../libs/Log -lgenericLog -L../libs/Memoire -lmemoire -L../libs/Threads -lthread -pthread -Wl,-rpath,'$$ORIGIN/../libs/Log' -Wl,-rpath,'$$ORIGIN/../libs/Threads' -Wl,-rpath,'$$ORIGIN/../libs/Memoire'

clean:
	rm -f $(TARGET)
