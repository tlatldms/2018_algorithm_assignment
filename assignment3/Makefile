CC = gcc

travel: travel.o tmpg.o
	gcc -o travel travel.o tmpg.o

tmpg.o: tmpg.c tmpg.h
	gcc -c tmpg.c

travel.o: travel.c tmpg.h
	gcc -c travel.c

clean:
	rm *.o travel
