.PHONY: stage1 stage2 stage3 stage4 build clean

stage1:
	gcc -E hello.c -o hello.i
stage2:
	gcc -S hello.i -o hello.S
stage3:
	gcc -c hello.S -o hello.o
stage4:
	gcc -o hello hello.o
build:
	gcc -o hello hello.c
clean:
	rm -rf hello.i hello.S hello.o