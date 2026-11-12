CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC

TARGET = libsaveLog.so libafficheLog.so libgenericLog.so

all: $(TARGET)

%.o: %.c *.h
	$(CC) $(CFLAGS) -o $@ -c $<

libsaveLog.so: libsaveLog.o
	$(CC) -shared -o $@ $<

libafficheLog.so: libafficheLog.o
	$(CC) -shared -o $@ $<

# rpath $ORIGIN : dlopen() cherche les strategies dans le dossier de libgenericLog.so
libgenericLog.so: libgenericLog.o
	$(CC) -shared -o $@ $< -ldl -Wl,-rpath,'$$ORIGIN'

clean:
	rm -f *.o *.so
