CC = gcc
CFLAGS = -g -Wall -lncurses

build:
	$(CC) -o battleship battleship.c $(CFLAGS)
run:
	./battleship harta1.txt harta2.txt harta3.txt
clean:
	rm battleship resume.txt