#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex06() {

    int i;
    int status;

    printf("A) Sâo criados 15 novos processos (1+2+4+8). \n");
    printf ("B) Mataria cada um dos filhos depois do SLEEP. \n");


    for (i = 0; i < 4; i++) {
        pid_t p = fork();
        if(p == 0) {
            sleep (1);
            // Retorna o número do processo por ordem de criação, excluindo o começo em 0
            exit(i+1);
        } else if (p > 0 && p % 2 == 0){
            waitpid(p, &status, 0);
        }
    }

}


int main(){
	pl1_ex06();
	return 0;
}
