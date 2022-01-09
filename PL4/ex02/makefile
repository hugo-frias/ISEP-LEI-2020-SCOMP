all: ex02 fillNumbers

ex02.o:	ex02.c
	gcc -Wall -g -c ex02.c -lrt -lpthread
	
ex02: ex02.o
	gcc -Wall -g ex02.o -o ex02 -lrt -lpthread
	
fillNumbers.o: fillNumbers.c
	gcc -Wall -g -c fillNumbers.c -lrt
	
fillNumbers: fillNumbers.o
	gcc -Wall -g fillNumbers.o -o fillNumbers -lrt
	
run_fillNumbers: fillNumbers
	./fillNumbers

run_ex02: ex02
	./ex02
	
clean:
	rm -rf *.o ex02 fillNumbers
