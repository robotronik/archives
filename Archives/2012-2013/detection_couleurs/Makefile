CFLAGS=`pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv` -lSDL_image

all:thread 


thread: thread.o couleur.o map.o
	gcc $^ -o $@ ${CFLAGS} ${LDFLAGS} -lm -l pthread


clean:
	rm -f *.o vision thread

couleur.o: couleur.c	
	gcc -c -std=c99  $^ -Wall ${CFLAGS} ${LDFLAGS} -lm

thread.o: thread.c
	gcc -c -std=c99  $^ -Wall ${CFLAGS} ${LDFLAGS} -lm -l pthread 

map.o: map.c
	gcc -c -std=c99  $^ -Wall ${CFLAGS} ${LDFLAGS} -lm -l pthread 


