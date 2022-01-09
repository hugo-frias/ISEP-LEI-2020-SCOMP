#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

#define NUM_IT 60
#define SLEEP_TIME 1

typedef struct {
    int direction;
		int west_direction;
		int east_direction;
		int direction_generator[NUM_IT];
} shared_data_type;


int pl4_ex16() {
    int i, aux, returnValue, fd, pidsaver[NUM_IT],data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
		sem_t *sem;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest16", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    /* Simple error handling */
    if (fd < 0) exit(1);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* Simple error handling */
    if (shared_data == NULL) exit(1);

		/* We create the semaphore */
		if ((sem = sem_open("sema16", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
 					perror("Error in sem_open()");
 				exit(1);
		}

		/* We initialize the struct data */
		shared_data->direction = rand() % 2;
		shared_data->west_direction = 0;
		shared_data->east_direction = 0;
		srand(time(NULL));
		/* We need to generate the random directions in the same process */
		for(i=0; i<NUM_IT; i++) {
			shared_data->direction_generator[i] = rand() % 2;
			sleep(1);
		}

		for(i=0; i<NUM_IT; i++) {
			/* The parent process creates a new child process */
			pidsaver[i] = fork();
			/* Only the child process will enter in this if */
			if(pidsaver[i] == 0) {
				aux =  shared_data->direction_generator[i];
				/* One direction */
				if(aux == 0) {
						/* If the current direction of the bridge is this */
						if(shared_data->direction == aux) {
							/* The time needed to croos the bridge */
							sleep(SLEEP_TIME);
							/* The usual control stuff */
							shared_data->west_direction++;
							printf("Passei para West \n");
						 /* In the case of the current directions is the opposite */
						} else {
							/* The car wait to all cars in the opposite way croos the bridge */
							sem_wait(sem);
							/* We change the current bridge's direction */
							shared_data->direction = aux;
							/* The time needed to croos the bridge */
							sleep(SLEEP_TIME);
							/* The usual control stuff */
							shared_data->west_direction++;
							printf("Passei para West \n");
							sem_post(sem);
						}
					/* The other direction */
				} else {
					/* If the current direction of the bridge is this */
						if(shared_data->direction == aux) {
							/* The time needed to croos the bridge */
							sleep(SLEEP_TIME);
							shared_data->east_direction++;
							printf("Passei para East \n");
							/* In the case of the current directions is the opposite */
						} else {
							/* The car wait to all cars in the opposite way croos the bridge */
							sem_wait(sem);
							/* We change the current bridge's direction */
							shared_data->direction = aux;
							/* The time needed to croos the bridge */
							sleep(SLEEP_TIME);
							/* The usual control stuff */
							shared_data->east_direction++;
							printf("Passei para East \n");
							sem_post(sem);
						}
				}
				exit(0);
			}
		}
	
	int status;
	/* Father waits for his childs */
	for(i=0; i<NUM_IT; i++) {
		waitpid(pidsaver[i], &status, 0);
	}

	/* The usual control stuff */
	printf("Número de East: \t %d \n", shared_data->east_direction);
	printf("Número de West: \t %d \n", shared_data->west_direction);

		/**
    * We unlink the semaphore created before
    */

		sem_unlink("sema16");

    /**
    * Disconnects the shared memory area from the process address space
    */

    returnValue = munmap(shared_data, data_size);
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    /**
    * Closes the file descriptor
    */

    returnValue = close(fd);
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    /**
    * Removes memory area from file system
    * May not be immediate if any process still has it open
    * The area is marked to be removed as soon as all processes close it
    */

    returnValue = shm_unlink("/shmtest16");
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    return 0;
}


int main(void) {
    int value = pl4_ex16();
    return value;
}