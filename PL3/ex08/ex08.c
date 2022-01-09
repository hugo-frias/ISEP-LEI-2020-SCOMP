#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define START_VALUE 100
#define NUMBER_OPERATION 1000000
#define shm_name "/shm_ex08"

int main(int argc, char *argv[])
{

    pid_t pid;
    int fd, data_size = sizeof(int);
    int *shared_data;
	
    if(shm_unlink("/shm_ex08")<0){ /* apaga a memoria partilhada do sistema */
        perror("No unlink()");   
    } 
        
    fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Erro ao abrir a memoria!\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, data_size) == -1)
    {
        perror("Erro a definir o tamanho!\n");
        exit(EXIT_FAILURE);
    }

    shared_data = (int *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("Erro no mmap!\n");
        exit(EXIT_FAILURE);
    }

    // Inicializar valor a 100
    *shared_data = START_VALUE;

    // Criação do processo filho e sua validação
    pid = fork();
    if (pid == -1)
    {
        perror("Erro");
    }

    // Código do processo pai
    if (pid > 0)
    {
        int i;
        for(i=0; i<NUMBER_OPERATION; i++){
            *shared_data++;
            *shared_data--;
            printf("Value at the end of the operation number PAIZAO: %d\n", *shared_data);
      
            }

			
        if (munmap(shared_data, 100) < 0)
        {
            exit(1);
        }

        if (close(fd) < 0)
        {
            exit(1);
        }
    }

    // Código do processo filho
    if (pid == 0)
    {
        int i;
        for(i=0; i<NUMBER_OPERATION; i++){
            *shared_data++;
			*shared_data--;
            printf("Value at the end of the operation number son: %d\n", *shared_data);
        
            }
			
        if (munmap(shared_data, 100) < 0)
        {
            exit(1);
        }

        if (shm_unlink(shm_name) < 0)
        {
            exit(1);
        }

        exit(0);
    }
    
    
   
    return 0;
}
