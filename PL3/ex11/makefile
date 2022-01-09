prog: ex11.o 
	gcc  ex11.o -o prog -lrt
	
	
ex11.o: ex11.c
	gcc -g -Wall -c ex11.c -o ex11.o
	
clean:
	rm -f *.o prog
	
run: prog
	./prog

