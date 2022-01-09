prog: ex06.o 
	gcc  ex06.o -o prog -lrt
	
	
ex06.o: ex06.c
	gcc -g -Wall -c ex06.c -o ex06.o
	
clean:
	rm -f *.o prog
	
run: prog
	./prog

