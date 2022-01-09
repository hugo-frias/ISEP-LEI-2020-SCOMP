#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex10() {
	time_t t;
	srand ((unsigned) time (&t));
	int x = rand() % 255;
	int i = 0, j = 0, k = 0, status, l = 0;
	int vec[2000];
	int vec1[10];
	for(i = 0; i<2000; i++) {
		*(vec+i) = rand() % 255;
	}
    for(j = 0; j<10; j++) {
		vec1[j] = fork();
		if (vec1[j] == 0) {
			for(k = j*200; k<200*(j+1); k++) {			
				if (vec[k]==x) {			
					exit(k-(200*j));					
				}
			}
			exit(255);
		}
    }
    for (l = 0; l<10; l++) {
    waitpid(vec1[l], &status, 0);
		if (WEXITSTATUS(status) != 255) {
			printf("index do array no filho %d : %d\n",l+1, WEXITSTATUS(status));
	    } else{
			printf("Não foi encontrado o valor no filho %d\n",l+1);
		}
	}
}

int main(){

	pl1_ex10();
	return 0;
}
