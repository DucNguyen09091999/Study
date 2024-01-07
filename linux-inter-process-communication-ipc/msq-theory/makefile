.PHONY: build clean

CFLAGS := -Wall -lrt
build:
	gcc -o sender -g sender.c $(CFLAGS)
	gcc -o receiver -g receiver.c $(CFLAGS)
clean:
	rm -rf sender receiver
