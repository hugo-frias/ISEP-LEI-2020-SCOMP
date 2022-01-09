#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define qtd_sems 2
#define shm_name "/shm_ex06"

typedef struct {
	int message;
}Shared_data;


int main(){
	sem_t *sem[qtd_sems];
    char sems_names[][25] = {"/semaforo_son_ex06", "/semaforo_father_ex06"};
    int sems_init_vales[] = {1, 0};
    enum index{
        SON,
        FATHER
    };
    int fd, data_size = sizeof(Shared_data);
    Shared_data *shared_data;
    
    fd = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd == -1){
		perror("Erro a abrir a memoria!\n");
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
    
    int i;
    for (i = 0; i < qtd_sems; i++){
		if ((sem[i] = sem_open(sems_names[i], O_CREAT | O_EXCL, 0644, sems_init_vales[i])) == SEM_FAILED){
            perror("No sem_open()");
            exit(1);
        }
    }
    
    pid_t pid = fork();
    if(pid == -1){
		perror("Erro no fork!\n");
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		while(shared_data -> message < 15){
			sem_wait(sem[SON]);
			printf("I'm the child!\n");
			shared_data -> message++;
			sem_post(sem[FATHER]);
		}
	}
	if(pid > 0){
		while(shared_data -> message < 14){
			sem_wait(sem[FATHER]);
			printf("I'm the father!\n");
			shared_data -> message++;
			sem_post(sem[SON]);
		}
	}					
	wait(NULL);
	
	sem_unlink(sems_names[SON]);
    sem_unlink(sems_names[FATHER]);
    
    if (munmap(shared_data, data_size) == -1){
		perror("Erro no munmap!\n");
        exit(EXIT_FAILURE);
    }

	shm_unlink(shm_name);	
	
	return 0;
}	
