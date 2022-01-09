#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_TRAVELS 5
#define DIFF_PATHS 3
#define MAX_WAIT 3

/* ASCII codes */
	enum cities {
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		CLEAR=0
	};

typedef struct {
	pthread_mutex_t mutex_saver[DIFF_PATHS];
} shared_data_type;

/*Path A to B */
void* travel_AB(void* arg){
	shared_data_type *shared_data = (shared_data_type*) arg;
		pthread_mutex_lock(&shared_data->mutex_saver[0]);
		int n = rand() % 3; sleep(n);
		printf("Comboio %lu passando de %d para %d. Tempo demorado: %d. \n", pthread_self(), A, B, n);
		pthread_mutex_unlock(&shared_data->mutex_saver[0]);
	
	pthread_exit((void*) NULL);
}	

	/*Path B to C */
void* travel_BC(void* arg){
	shared_data_type *shared_data = (shared_data_type*) arg;
		pthread_mutex_lock(&shared_data->mutex_saver[1]);
		int n = rand() % 3; sleep(n);
		printf("Comboio %lu passando de %d para %d. Tempo demorado: %d. \n", pthread_self(), B, C, n);
		pthread_mutex_unlock(&shared_data->mutex_saver[1]);

		pthread_exit((void*) NULL);
}	

	/*Path B to D */
void* travel_BD(void* arg){
	shared_data_type *shared_data = (shared_data_type*) arg;
		pthread_mutex_lock(&shared_data->mutex_saver[2]);
		int n = rand() % 3; sleep(n);
		printf("Comboio %lu passando de %d para %d.Tempo demorado: %d. \n", pthread_self(), B, D, n);
		pthread_mutex_unlock(&shared_data->mutex_saver[2]);
	
	pthread_exit((void*) NULL);
}	

int main(){
	int i, data_size = sizeof(shared_data_type);
	unsigned int random_path;
	pthread_t thread[NUM_TRAVELS];
	shared_data_type *shared_data = (shared_data_type*) malloc(data_size);

		/*We initialize the mutex variables */
    for (i = 0; i < DIFF_PATHS; i++) {
        pthread_mutex_init(&shared_data->mutex_saver[i], NULL);
    }



	/* We generate random travels and then we call the correspondent function */
	for(i=0; i<NUM_TRAVELS; i++) {
		srand(i);
		random_path = rand() % 3;
		switch(random_path) {
			case 0: pthread_create(&thread[i], NULL, travel_AB, (void *) shared_data); break;
			case 1: pthread_create(&thread[i], NULL, travel_BC, (void *) shared_data); break;
			case 2: pthread_create(&thread[i], NULL, travel_BD, (void *) shared_data); break;
		}
	}


	/* We wait for all the travels */
	for(i=0; i<NUM_TRAVELS; i++) {
			pthread_join(thread[i], NULL);
	}

	free(shared_data);
	return 0;
}