CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra
LDLIBS = -lm

VPATH = src headers
OBJ = main.o preproc.o misc.o BnB.o p_center.o

all: main

main: $(OBJ)

.PHONY: clean

clean:
	$(RM) *.o main