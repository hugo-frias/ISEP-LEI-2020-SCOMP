#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>

#define shm_name "/shm_ex13"

typedef struct{
	int occurences[10];
	char word[10][255];
	char path[10][100];
} Memoria_partilhada;


int main(int argc, char **argv){
	int fd;
	int data_size = sizeof(Memoria_partilhada), i;
	Memoria_partilhada *memoria;
	FILE *f;
	char line[1024];
	pid_t pid;
	
	fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Erro ao abrir a memoria!\n");
		exit(EXIT_FAILURE);
	}
	
	if(ftruncate(fd, data_size) == -1){
		perror("Erro a definir o tamanho!\n");
		exit(EXIT_FAILURE);
	}	
	
	memoria = (Memoria_partilhada *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(memoria == MAP_FAILED){
		perror("Erro no mmap!\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(memoria -> word[0], "este");
	strcpy(memoria -> word[1], "é");
	strcpy(memoria -> word[2], "teste");
	strcpy(memoria -> word[3], "um");
	strcpy(memoria -> word[4], "executado");
	strcpy(memoria -> word[5], "por");
	strcpy(memoria -> word[6], "memoria");
	strcpy(memoria -> word[7], "partilhada");
	strcpy(memoria -> word[8], "entre");
	strcpy(memoria -> word[9], "processos");
	
	for(i = 0; i < 10; i++){
		sprintf(memoria -> path[i], "ficheiro%d.txt", i);
		pid = fork();
		if(pid == -1){
			perror("Erro no fork!\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			f = fopen(memoria -> path[i], "r");
			while(fgets(line, sizeof(line), f) != NULL){
				if(strstr(line, memoria -> word[i]) != NULL){
					memoria -> occurences[i]++;
				}
			}
			fclose(f);
			exit(0);
		}
	}
	
	while(wait(NULL) > 0);
	
	for(i = 0; i < 10; i++){
		printf("O filho %d encontrou a palavra '%s' %d vezes no ficheiro %s!\n", i, memoria -> word[i], memoria -> occurences[i], memoria -> path[i]);
	}
	
	if(munmap(memoria, data_size) == -1){
		perror("Erro no munmap!\n");
		exit(EXIT_FAILURE);
	}
		
	shm_unlink(shm_name);					
	return 0;
}
