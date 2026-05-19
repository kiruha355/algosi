CC = gcc
FLAG = -Werror -Wall -std=c11

build: avl.o main.o
	${CC} ${FLAG} avl.o main.o -o avl -lm

test: avl.o test_avl.o
	${CC} ${FLAG} avl.o test_avl.o -o test_avl -lm
	./test_avl

avl.o: avl.c avl.h
	${CC} ${FLAG} -c avl.c

main.o: main.c avl.h
	${CC} ${FLAG} -c main.c

test_avl.o: test_avl.c avl.h
	${CC} ${FLAG} -c test_avl.c

run: build
	./avl -f input.txt output.txt

clean:
	-del /Q *.o avl.exe test_avl.exe output.txt 2>nul