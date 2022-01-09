#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>


int main(){
	FILE *file;
	int i;
	
	file = fopen("Numbers.txt", "w");
	for(i = 0; i < 1600; i++){
		fprintf(file, "%d", i);
		fprintf(file, "\n");
	}
	
	fclose(file);	
	
	return 0;
}

