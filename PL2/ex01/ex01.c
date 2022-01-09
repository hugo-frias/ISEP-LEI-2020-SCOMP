#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void pl2_ex1() {
    int fd[2];  //descritor
    pid_t p;

    //criar o pipe, antes de criar o filho, para ser o mesmo pipe no pai e no filho
    if(pipe(fd)==-1){
		perror("Pipe failed");
		//return 0;
	}
    
    if(fork()==0){ //filho
        close(fd[1]); //fecha a escrita
        read(fd[0], &p, sizeof(p)); //lê do pipe com o descritor (ESPERA que exista algo para ler)
        printf("O filho leu do pipe: %d\n", p); //escreve no ecra
        close(fd[0]); //fecha a leitura
        exit(0);
    }
    else{
        p=getpid();
        printf("Pai: %d\n", getpid());
        close(fd[0]); //fecha a leitura
        write(fd[1], &p, sizeof(p)); //escreve no pipe com o descritor
        close(fd[1]); //fecha a escrita
    }
    wait(NULL);
}