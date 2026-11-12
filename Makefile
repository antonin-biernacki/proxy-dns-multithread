.PHONY: all build clean

all: build

build:
	@mkdir -p libs/static
	@mkdir -p bin
	$(MAKE) -C libs/Memoire
	$(MAKE) -C libs/Threads
	$(MAKE) -C libs/Reseau
	$(MAKE) -C libs/Log
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean
	$(MAKE) -C libs/Reseau clean
	$(MAKE) -C libs/Log clean
	$(MAKE) -C libs/Threads clean
	$(MAKE) -C libs/Memoire clean
	rm -df bin
	rm -df libs/static
