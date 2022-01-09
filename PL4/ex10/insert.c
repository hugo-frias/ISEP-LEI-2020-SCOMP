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
	User user;
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

	sem_wait(sems[shared_data->next]);
	
	write(1, "Name:\n", strlen("Name:\n"));
	scanf("%s", user.name);
	write(1, "Number:\n", strlen("Number:\n"));
	scanf("%d", &user.number);
	write(1, "Address:\n", strlen("Address:\n"));
	scanf("%s", user.address);
	if (shared_data -> next == 100){
		 write(1, "No more space\n", strlen("No more space\n"));
	}	 
	shared_data -> records[shared_data -> next] = user;
	sem_post(sems[shared_data->next]);
	
	shared_data -> next++;
	sem_post(sems[shared_data->next]);
	
	if (munmap(shared_data, data_size) == -1){
		perror("Erro no munmap!\n");
        exit(EXIT_FAILURE);
    }
    
    close(fd);	
    
	return 0;
}	
