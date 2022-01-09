prog: ex04.o 
	gcc  ex04.o -o prog -lrt
	
	
ex04.o: ex04.c
	gcc -g -Wall -c ex04.c -o ex04.o
	
clean:
	rm -f *.o prog
	
run: prog
	./prog

