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

#define shm_name "/shm_ex10"
#define sem_name "/sem_ex10"

typedef struct{
	int number;
	char name[100];
	char address[150];
}User;

typedef struct{
	User records[100];
	int next;
} Shared_data;

int main(){
	int fd, data_size = sizeof(Shared_data);
	Shared_data *shared_data;
	sem_t *sems[100];
	char *nameSems[100];
	int i;
	
	fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1){
		perror("Erro ao abrir a memoria!\n");
		exit(EXIT_FAILURE);
	}
	
	if (ftruncate(fd, data_size) == -1){
       perror("Erro a definir o tamanho!\n");
       exit(EXIT_FAILURE);
    }
    
    shared_data = (Shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED){
        perror("Erro no mmap!\n");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0; i < 100; i++){
		nameSems[i] = (char *) malloc(100);
		sprintf(nameSems[i], "%s%d",sem_name,i);
		sems[i] = sem_open(nameSems[i], O_CREAT | O_RDWR, 0644, 1);
	}

    // Fechar a memória partilhada
    if (munmap(shared_data, 100) < 0)
    {
        exit(1);
    }
    
    if (shm_unlink(shm_name) < 0)
    {
        exit(1);
    }

    // Fechar e remover os semáforos
    for (i = 0; i < 100; i++)
    {
        sem_close(sems[i]);
        sem_unlink(nameSems[i]);
    }

    return 0;
}
	
	
