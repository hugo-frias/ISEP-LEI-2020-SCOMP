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

typedef struct {
    int superiorNumerator;
} shared_data_type;

int num;

/* Adaptation from: https://bit.ly/2Z7CcAD */
void* prime_numbers(void* arg){
	int i, controll, flag = 0;
	int value = *(int*) arg;

	printf("Números primos até %d: ", value);
	for(controll=0; controll<value; controll++) {
	 for (i = 2; i <= controll / 2; ++i) {
        if (controll % i == 0) {
            flag = 1;
            break;
        }
    }
    if (controll == 1) {
        continue;
    }
    else {
        if (flag == 0)
            printf("%d ", controll);
    }
	}
	printf(".\n");
	pthread_exit((void*) NULL);
}					

int main(){
	int data_size = sizeof(shared_data_type);
	pthread_t thread;
	shared_data_type *shared_data = (shared_data_type*) malloc(data_size);
	printf("Por favor, introduza um número: ");
	scanf("%d", &shared_data->superiorNumerator);
	pthread_create(&thread, NULL, prime_numbers, (void*) shared_data);
	pthread_join(thread, NULL);
	
	free(shared_data);
	return 0;
}