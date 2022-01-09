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

#define SEMAPHORES_SIZE 3

void buyChips(){
	printf("buys chips\n");
}

void buyBeer(){
	printf("buys beer\n");
}

void eat_and_drink(){
	printf("eats and drinks\n");	
}

void pl4_ex09(){
	
    sem_t *sem[SEMAPHORES_SIZE]; /* ap para o semaforo */

    int i;
    char name[50];
  
	for (i = 0; i < SEMAPHORES_SIZE; i++) {
		sprintf(name, "/pl04_ex09_sem%d", i);
		if ((sem[i] = sem_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO, 1)) < 0)
			perror("Failed to open semaphore");
	} 



    
    /*semáforos: 
     * sem(0) - semaforo das batatas - inicia a 1
     * sem(1) - semáforo da cerveja - inicia a 1
     * sem(2) - semáforo para dar check - inicia a 1*/
    
    
    /*process 1 - filho; process 2 - pai */
		int pid = fork();
	    if(pid==0){	/*filho*/
		sem_wait(sem[0]); //batatas 0
		buyChips();
		int checkOutraComida = 0;
		int checkJaComeu = 0;
		sem_getvalue(sem[1], &checkOutraComida);
		sem_getvalue(sem[2], &checkJaComeu);
		if(checkOutraComida == 0 && checkJaComeu == 1){ //caso beer comprada e ainda nao tiver comido e bebido
			sem_wait(sem[2]);//come e bebe a 0
			eat_and_drink();
		}
     
		
	} else{
		sem_wait(sem[1]); //beer a 1
		buyBeer();
		int check = 0;
		int checkJaComeu = 0;
		sem_getvalue(sem[2], &checkJaComeu);
		sem_getvalue(sem[0], &check);
		if(check == 0 && checkJaComeu == 1){ //caso batatas compradas e ainda nao tiver comido e bebido
			sem_wait(sem[2]);//come e bebe a 0
			eat_and_drink();
		}
		int status;
		waitpid(pid, &status, 0);

		
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

    


	}


int main(int argc, char *argv[]) {
    pl4_ex09();
    return 0;
}
	
