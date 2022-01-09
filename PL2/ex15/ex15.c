#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Copiada da internet
int factorial(int n) {
  int c, f = 1;
  
  for (c = 1; c <= n; c++) {
    f = f * c; 
   }
  return f;
}

int pl2_ex15() {
  int i, pipeVerify, p, number, auxiliarPipeIDSaver[2], pipeIDSaver[4];
  
  //Criamos 2 PIPES (2*2=4)
  for(i=0; i<4; i=i+2) {
        pipeVerify = pipe(auxiliarPipeIDSaver);
        if(pipeVerify == -1) {
            return -1;
        } else {
            pipeIDSaver[i] = auxiliarPipeIDSaver[0];
            pipeIDSaver[i+1] = auxiliarPipeIDSaver[1];
        }
    }


  //Criamos o filho
  p = fork();

  if(p>0) {
    // Fechamos as extremidades não usadas
    close(pipeIDSaver[1]);
    close(pipeIDSaver[2]);
    printf("Introduza um inteiro (Pai %d): \n", getpid());
    scanf("%d", &number);
    write(pipeIDSaver[3], &number, sizeof(int));
    close(pipeIDSaver[3]);
    int status;
    int fatorialNumber;
    //waitpid(p, &status, 0);
    read(pipeIDSaver[0], &fatorialNumber, sizeof(int));
    close(pipeIDSaver[0]);
    printf("Fatorial (Pai %d): \t %d \n", getpid(), fatorialNumber);
  } else if (p == 0) {
    //Fechamos as extremidades não usadas
    close(pipeIDSaver[0]);
    close(pipeIDSaver[3]);
    // Lemos o número do Pai
    read(pipeIDSaver[2],&number,sizeof(int));
    close(pipeIDSaver[2]);
    long fatorial = factorial(number);
    write(pipeIDSaver[1], &fatorial, sizeof(long));
    close(pipeIDSaver[1]);
  }
  return 0;
}



int main() {
  return pl2_ex15();
}