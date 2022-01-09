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

static int num_filhos = 8;
static int num_sems = 8;

int main(){
	int i, j, status, number[1600], out, lineNumber = 0;
	char name[124];
	sem_t *sem_n[num_sems], *sem_out;
	FILE *f_n, *f_out;
	pid_t pid;

	for (i = 0; i < num_sems; i++){
		sprintf(name, "numbers%d", i);
		if (i == 0){
			if ((sem_n[i] = sem_open(name, O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
				perror("Erro a abrir o semaforo!\n");
				exit(EXIT_FAILURE);
			}
		} else {
			if ((sem_n[i] = sem_open(name, O_CREAT|O_EXCL, 0644, 0)) == SEM_FAILED) {
				perror("Erro a abrir o semaforo!\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	if ((sem_out = sem_open("output", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a abrir o semaforo!\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < num_filhos; i++){
		pid = fork();
		if (pid == -1){
			perror("Erro no fork!\n");
			exit(EXIT_FAILURE);
		} 
		
		if (pid == 0){
			sem_wait(sem_n[i]);
			f_n = fopen("numbers.txt", "r");
			for(j = i*200; j < (i+1)*200; j++){
				while(lineNumber != i*200){
					fscanf(f_n, "%d", &number[lineNumber]);
					lineNumber++;
				}
				fscanf(f_n, "%d", &number[j]);
			}
			fclose(f_n);
			
			if (i != 7){
				sem_post(sem_n[i+1]);
			}
			sem_wait(sem_out);
			f_out = fopen("output.txt", "a");
			for(j = i*200; j < (i+1)*200; j++){
				fprintf(f_out, "%d\n", number[j]);
			}
			fclose(f_out);
			sem_post(sem_out);
			exit(0);
		}
	}
	
	while(wait(&status) > 0);
	
	f_out = fopen("output.txt", "r");
	while (fscanf(f_out, "%d", &out) != EOF){
		printf("%d\n", out);
	}
	fclose(f_out);

	for (i = 0; i < num_sems; i++){
		sprintf(name, "numbers%d", i);
		sem_unlink(name);
	}

	sem_unlink("output");
	
	return 0;
}

