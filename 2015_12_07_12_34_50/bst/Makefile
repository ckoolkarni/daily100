CC=gcc
CCFLAGS=-Wall -Werror -g
LDFLAGS=

bst.o: bst.c bst.h
	${CC} ${CCFLAGS} ${LDFLAGS} -c $< -o $@

.PHONY:clean

clean:
	rm -fr bst.o
