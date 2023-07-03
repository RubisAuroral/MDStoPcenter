CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -g -O3
LDLIBS = -lm

VPATH = src headers
OBJ = main.o preproc.o misc.o BnB.o p_center.o pcenter-to-mds.o main-emos-mds.o

all: main

main: $(OBJ)

.PHONY: clean

clean:
	$(RM) *.o main