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
	int i;
	Shared_data *shared_data;
		
	fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
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
	
	
	for(i = 0; i < shared_data -> next; i++){
		printf("\nNumber: %d\n",shared_data->records[i].number);
		printf("Name: %s\n",shared_data->records[i].name);
		printf("Address: %s\n",shared_data->records[i].address);
	}	
	
	if (munmap(shared_data, data_size) == -1){
		perror("Erro no munmap!\n");
        exit(EXIT_FAILURE);
    }
    
    close(fd);
    
    return 0;
}
    
