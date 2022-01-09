#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex11() {
    int i, j, processAuxID, maxValue = 0, status, parentMaxValueStatus, parentMaxTrueValue = 0;
    int vec[1000], p[5], result[1000];

    // Inicializamos o vetor com 1000 posições
    for(i=0; i<1000; i++) {
        // Gera um número aleatório de 0 a 255
        vec[i] = rand() % 256;
    }

    for(i=0; i<5; i++) {
        p[i] = fork(); //Cria um novo processo
        if(p[i] == 0) {
            maxValue=0;
            for(j=i*200; j<(i+1)*200; j++) {
                if(vec[j] > maxValue) {
                    maxValue = vec[j];
                }
            }
            exit(maxValue); // Garantimos que o processo filho faz algumas operações e depois é terminado
        }
        
	}

    // Calculamos o valor máximo das 5 partes (array completo)
    for(i=0; i<5; i++) {
			waitpid(p[i], &status,0);
			parentMaxValueStatus = WEXITSTATUS(status);
			if(parentMaxValueStatus > parentMaxTrueValue) {
			parentMaxTrueValue = parentMaxValueStatus;
			}
		}


    // O filho faz o cálculo na primeira metade do array
    processAuxID  = fork();
    if(processAuxID == 0) {
        for(i=0; i<500; i++) {
            result[i] =  (int) (((double) vec[i] / parentMaxTrueValue) * 100);
        }

        for(i=0; i<500; i++) {
            printf("Value %d : %d \n", i, result[i]);
        }

    } else if (processAuxID >0) {
           // O pai faz o cálculo da segunda metade do array em "simultâneo" com o filho
           // Mas espera que o filho termine de executar para proceder à impressão da sua parte do array
              for(i=500; i<1000; i++) {
                  result[i] = (int) (((double) vec[i] / parentMaxTrueValue) * 100);
              }

              j = wait(NULL);
              for(i=500; i<1000; i++) {
                  printf("Value %d : %d \n", i, result[i]);
              }
          }
}

int main(){
	pl1_ex11();
	return 0;
}
