all: consult insert consult_all delete
	
consult: consult.c
	gcc -Wall -g consult.c -o consult -lrt -lpthread
	
consult_all: consult_all.c
	gcc -Wall -g consult_all.c -o consult_all -lrt -lpthread	
	
insert: insert.c
	gcc -Wall -g insert.c -o insert -lrt -lpthread

delete: delete.c
	gcc -Wall -g delete.c -o delete -lrt -lpthread
	
run_insert: insert
	./insert

run_consult: consult
	./consult
	
run_consult_all: consult_all
	./consult_all	
	
clean:
	rm -rf consult insert consult_all delete
