#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

struct vec_numbers {
    int initialized_vector[1000];
    int maximum_vector[10];
};


int pl3_ex11() {
    int i, j, status;
    int pid_saver[10];
    
    struct vec_numbers vecnumbers;
	struct vec_numbers *shared_data = &vecnumbers;
	
	
	//Create 1 pipe
	int fd[2];
	if(pipe(fd) == -1) {
		perror("Erro ao criar pipe!");
	return 1;
	}

	
  
    /**
    * We fullfield the initialized_vector with some random integers
    */
	srand(time(NULL));
    for (i = 0; i < 1000; i++) {
        shared_data->initialized_vector[i] = rand() % 1000;
    }


    /**
    * Father creates 10 childs
    */

    for (i = 0; i < 10; i++) {
        pid_saver[i] = fork();
        if (pid_saver[i] < 0) exit(1);

            /**
            * Each child calculates the local maximum value
            */

        else if (pid_saver[i] == 0) {
            int maximum = 0;
            for (j = i * 100; j <= (i + 1) * 100; j++) {
                if (shared_data->initialized_vector[j] > maximum) {
                    maximum = shared_data->initialized_vector[j];
                }
            }
            close(fd[0]); //fecha a leitura
            write(fd[1], &maximum, sizeof(maximum));
            close(fd[1]);

            /**
            * We kill the child after all operations to make sure that only father creates new processes
            */

            exit(0);
        }
    }

    /**
    * Father waits for all childs
    */

    for (i = 0; i < 10; i++) {
        j = waitpid(pid_saver[i], &status, 0);
    }

    /**
   * Prints array with all maximum values
   */
	int aux = 0;
	int max = 0;
	close(fd[1]);
	while(read(fd[0], &aux, sizeof(aux))>0){
    if (aux>max) {
            max = aux;
        }
	}
	close(fd[0]);
	
	printf("\nMaximum value found: %d \n", max);
return 0;
}
	
int main(void) {
    int value = pl3_ex11();
    return value;
}
