#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

void pl2_ex4() {
    int fd[2];  //descritor
    char frase[50];

    pipe(fd); //pipe, antes de criar o filho para ser o mesmo

    if(fork()==0){ //filho
        close(fd[1]); //fecha a escrita
        read(fd[0], frase, sizeof(frase)+1); //le do pipe com o descritor
        printf("O filho leu do pipe a string: %s\n", frase); //escreve no ecra
        close(fd[0]); //fecha a leitura
        exit(0);
    }
    else{
        close(fd[0]); //fecha a leitura
        int c, i=0;
        FILE *fp=fopen("/home/hsp/Desktop/fich.txt", "r"); //ALTERAR PWD DO FICH!!!
        while(1) {

            /**
            *
            * A função de escrita (supostamente) é:
            * fprint(pointer, "%d%s%f%c ...", a,b,c,d)
            */
	
            c = fgetc(fp);

            /**
             *  Testa se chegámos ao final do ficheiro
             *  Se sim, retorna 1
             *  Em C, o 0 é falso
             */

            if(feof(fp)) {
                break ;
            }
            frase[i]=c; //Absorve um caracter
            i++;
        }
        fclose(fp);
        write(fd[1], frase, sizeof(frase) + 1); //escreve no pipe com o descritor
        close(fd[1]); //fecha a escrita
    }
    wait(NULL);
}

int main(){
    pl2_ex4();
    return 0;
}