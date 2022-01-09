#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define ARRAY_SIZE 1000000

int main() {
	int fd;
	int pip[2];
	clock_t start, end;
	int  s, i;
	int numbers[ARRAY_SIZE];
	int* numbersP;

	for (i = 0; i < ARRAY_SIZE; i++) {
		srand(time(0));
		numbers[i] = rand() % 10;
	}

    ///////SHM
	/* read,write,execute/search permissions by owner and group */
	if((fd = shm_open("/shmHSP", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) <0)
        perror("Failed to create shared memory");

    /* Ajusta o tamanho da memória partilhada */
    if(ftruncate(fd, sizeof(numbers))<0)
        perror("Failed to adjust memory size");

    /* Mapea a memória partilhada */
    numbersP = mmap(NULL, sizeof(numbers), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* Verifica se a memória partilhada foi devidamente mapeada */
    if (numbersP == NULL) {
        perror("No mmap()");
        exit(1);
    }

	for (i = 0; i < ARRAY_SIZE; i++) {
		*(numbersP+i)=numbers[i];
	}
	start = clock();

	if (fork() == 0) {
		for (i = 0; i < ARRAY_SIZE; i++)
			numbers[i] = *(numbersP+ i);
		exit(0);
	}

	wait(&s);
	end = clock();
	printf("Shared memory took %f sec \n",
			((double) (end - start)) / CLOCKS_PER_SEC);

    if(close(fd)<0){ /* desfaz mapeamento */
        perror("No close()");
        exit(1);
    }

    if(munmap(numbersP, sizeof(numbersP))<0){ /* desfaz mapeamento */
        perror("No munmap()");
        exit(1);
    }

    if(shm_unlink("/shmHSP")<0){ /* apaga a memoria partilhada do sistema */
        perror("No unlink()");
        exit(1);
    }

    ///////PIPE

	if (pipe(pip) == -1) {
        perror("Pipe failed");
        return 1;
    }

	start = clock();

	if(fork()==0){
		close(pip[0]);
		write(pip[1],numbers,sizeof(numbers));
		close(pip[1]);
		exit(0);
	}

	int aux[ARRAY_SIZE];

	read(pip[0],aux,sizeof(aux));

	end = clock();
	printf("Pipe took %f sec \n",
				((double) (end - start)) / CLOCKS_PER_SEC);

	return 0;
}
