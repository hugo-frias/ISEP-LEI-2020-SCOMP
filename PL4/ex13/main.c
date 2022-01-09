
/*  Exemplo de semaforo binario.
 *
 *  Utilizacao de semaforo binario para controlar acessos a memoria partilhada
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SEMAPHORES_SIZE 2

typedef struct {
    int buffer_vector[10];
    int indicator;
} shared_data_type;

int create_childs(int nrFilhos){
    int i;    
    pid_t pid;
    for (i = 0; i < nrFilhos; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            return 1;
        }
    }
    return 0;
}

void pl4_ex13(){
	int i, r,fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest11", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    sem_t *sem[SEMAPHORES_SIZE]; /* ap para o semaforo */

    char name[50];
  
	for (i = 0; i < SEMAPHORES_SIZE; i++) {
		sprintf(name, "/pl04_ex09_sem%d", i);
		if ((sem[i] = sem_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO, 1)) < 0)
			perror("Failed to open semaphore");
	} 
	sem_wait(sem[0]);


    
    /*semáforos: 
     * sem(0) - semaforo buffer cheio para pai
     * sem(1) - semáforo buffer cheio para filho*/
    shared_data->indicator = 0;
    int j;
    for (j = 0; j < 10 ; j++){ 
       shared_data->buffer_vector[j] = 0; 
    } 
		
		int pidAux =  create_childs(2);
		int valor = 0;
		int counterLoop= 0; 
		int check = 0;
		for(counterLoop = 0; counterLoop<3; counterLoop++){
	    if(pidAux==0){	/*pai*/
		sem_wait(sem[0]); 
		while(shared_data->indicator!=0){
			
			printf("Eu (pai) li o valor %d na ronda %d \n", shared_data->buffer_vector[shared_data->indicator], counterLoop);
			shared_data->indicator--;
			}	
			sem_post(sem[1]);
			sem_post(sem[1]);
			
		} else{ /*filho*/
			while(shared_data->indicator!=9){
				shared_data->buffer_vector[shared_data->indicator] = valor;
				valor++;
				shared_data->indicator++;
			} 
			if(check != 1){
				sem_post(sem[0]);
				check = 1;
			}
			sem_wait(sem[1]);
			check = 0;
				
			}
		}

	/* fechar/apagar os semaforos */
	 for (i = 0; i < SEMAPHORES_SIZE; i++) {
        if(sem_close(sem[i]) < 0) {
            perror("sem_close()");
            exit(EXIT_FAILURE);
        }
        sprintf(name, "/pl04_ex09_sem%d", i);
        if (sem_unlink(name) < 0) {
            perror("sem_unlink()");
            exit(EXIT_FAILURE);
        }
    }
    /* desfaz mapeamento */
    r=munmap(shared_data, data_size);
    if (r < 0) { /* verifica erro */
        perror("No munmap()");
        exit(1);
    }
    
    /* apaga a memoria partilhada do sistema */
    r=shm_unlink("//hsp_shm");
    if (r < 0) { /* verifica erro */
        perror("No unlink()");
        exit(1);
    }
    


	}


int main(int argc, char *argv[]) {
    pl4_ex13();
    return 0;
}
	
