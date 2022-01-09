#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl2_ex2a() {
    int fd[2];  //descritor
    char frase[50];
    int valor;

    pipe(fd); //pipe, antes de criar o filho, para ser o mesmo pipe no pai e no filho

    if(fork()==0){ //filho
        close(fd[1]); //fecha a escrita
        read(fd[0], &valor, sizeof(valor)); 
        read(fd[0], &frase, sizeof(frase)+1); 
        printf("O filho leu do pipe o valor: %d\n", valor); 
        printf("O filho leu do pipe o valor: %s\n", frase); 
        close(fd[0]); //fecha a leitura
        exit(0);
    }
    else{
        printf("Qual o valor numérico?"); scanf("%d", &valor); 
        printf("Qual a frase?"); fflush(stdin); scanf("%s", frase); fflush(stdin);
        close(fd[0]); //fecha a leitura
        write(fd[1], &valor, sizeof(valor)); 
        write(fd[1], frase, sizeof(frase)+1); 
        close(fd[1]); //fecha a escrita
    }
    wait(NULL);
}

void pl2_ex2b() {
    int fd[2];  //descritor
    struct sconteudo {
        char frase[50];
        int valor;
    } conteudo; 
    
    pipe(fd); //pipe, antes de criar o filho, para ser o mesmo pipe no pai e no filho

    if(fork()==0){ //filho
        close(fd[1]); //fecha a escrita
        read(fd[0], &conteudo.valor, sizeof(conteudo.valor)); 
        read(fd[0], &conteudo.frase, sizeof(conteudo.frase)+1); 
        printf("O filho leu do pipe o valor: %d\n", conteudo.valor); 
        printf("O filho leu do pipe o valor: %s\n", conteudo.frase); 
        close(fd[0]); //fecha a leitura
        exit(0);
    }
    else{
        printf("Qual o valor numérico?"); scanf("%d", &conteudo.valor); 
        printf("Qual a frase?"); fflush(stdin); scanf("%s", conteudo.frase); fflush(stdin);
        close(fd[0]); //fecha a leitura
        write(fd[1], &conteudo.valor, sizeof(conteudo.valor)); 
        write(fd[1], conteudo.frase, sizeof(conteudo.frase)+1); 
        close(fd[1]); //fecha a escrita
    }
    wait(NULL);
}



int main(){
    pl2_ex2a();
    pl2_ex2b();
    return 0;
}
