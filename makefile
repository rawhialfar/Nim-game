all: nim
nim: nim.o alphanim.o nimhelp.o
	clang -Wall -pedantic -std=c99 nim.c alphanim.c nimhelp.c
nim.o: nim.c nim.h
	clang -Wall -pedantic -std=c99 -c nim.c
nimhelp.o: nimhelp.c nim.h
	clang -Wall -pedantic -std=c99 -c nimhelp.c
alphanim.o:  alphanim.o nim.h nim.o nimhelp.o
	clang -Wall -pedantic -std=c99 alphanim.c nim.o nimhelp.o
clean:
	rm *.o
