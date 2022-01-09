#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define DESC_LENGTH 20

/* WRITER */

typedef struct {
	int product_code;
	char description [DESC_LENGTH];
	double price;
} shm_data_type;

int main() {
	int fd, r;
	shm_data_type *shm_data;

	/* read,write,execute/search permissions by owner and group */
	fd = shm_open("/shmHSP", O_CREAT|O_RDWR, S_IRWXU|S_IRWXG);

	/* Verifica se a memória partilhada foi devidamente aberta */
	if (fd < 0) {
		perror("No smh_open!");
		exit(0);
	}

	/* Ajusta o tamanho da memória partilhada */
	ftruncate(fd, sizeof(shm_data_type));

	/* Mapea a memória partilhada */
	shm_data = (shm_data_type *)mmap(NULL, sizeof(shm_data_type), PROT_READ|PROT_WRITE,
				MAP_SHARED, fd, 0);

	/* Verifica se a memória partilhada foi devidamente mapeada */
	if (shm_data == NULL) {
		printf("No mmap()");
		exit(0);
	}
	

	/* É escrito o número do aluno na memória partilhada pela consola */
	printf("\nInsert the product's code: ");
	scanf("%d", &(shm_data->product_code));
	printf("\n");
	
	
	/* É escrito o nome do aluno na memória partilhada pela consola */
	printf("\nInsert the product's description: ");
	scanf("%s", &(shm_data->description));
	printf("\n");

	/* É escrito o número do aluno na memória partilhada pela consola */
	printf("\nInsert the product's price: ");
	scanf("%lf", &(shm_data->price));
	printf("\n");

	/* Desfaz o mapeamento */
	r = munmap(shm_data, sizeof(shm_data));

	if (r < 0) {
		perror("No munmap()!");
		exit(1);
	}

	return 0;
}
