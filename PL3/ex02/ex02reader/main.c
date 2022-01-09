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
/* READER */

typedef struct {
	int product_code;
	char description [DESC_LENGTH];
	double price;
} shm_data_type;

int main() {
	int fd, r;
	shm_data_type *shm_data;

	/* read,write,execute/search permissions by owner and group */
	fd = shm_open("/shmHSP", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);

	/* Verifica se a memória partilhada foi devidamente aberta */
	if (fd < 0) {
		perror("No smh_open!");
		exit(0);
	}

	/* Ajusta o tamanho da memória partilhada */
	ftruncate(fd, sizeof(shm_data_type));

	/* Mapea a memória partilhada */
	shm_data = (shm_data_type *) mmap(NULL, sizeof(shm_data_type),
			PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/* Verifica se a memória partilhada foi devidamente mapeada */
	if (shm_data == NULL) {
		printf("No mmap()");
		exit(0);
	}

	/* São imprimidos o nome e número do aluno lidos da memória partilhada */
	printf("\nNumProduto = %d", shm_data->product_code);
	printf("\nDescricao = %s", shm_data->description);
	printf("\nPreço = %lf", shm_data->price);
	printf("\n");

	/* Desfaz o mapeamento */
	r = munmap(shm_data, sizeof(shm_data));
	/* Verifica se o mapeamento foi desfeito com sucesso */
	if (r < 0) {
		perror("No munmap()!");
		exit(1);
	}

	/* Apaga a memória partilhada do sistema */
	r = shm_unlink("/shmHSP");
	/* Verifica se a memória partilhada foi apagada do sistema com sucesso */
	if (r < 0) {
		perror("No unlink()!");
		exit(1);
	}

	return 0;
}
