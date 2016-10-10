all: bstsort

find: bstsort.o
	gcc -o bstsort bstsort.o

find.o: bstsort.c
	gcc -c bstsort.c -o bstsort.o

clean:
	rm -f bstsort bstsort.o find core *~ 
