#makefile do trabalho A2 prog2 

FLAGS=	-Wall	-g

main:	main.c	log.c	arff.c	arff.h	lista.c	lista.h

	$(CC)	$(FLAGS)	main.c	log.c	arff.c	lista.c	-o	main

clean:
	-rm	-f	*.o

purge:	clean
	-rm	-f	main
