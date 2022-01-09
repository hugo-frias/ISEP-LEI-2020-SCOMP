#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl2_ex10() {
    int fd[2];  //descritor
    int i,y, x, credit = 20;
    int instruction, status, bet;
    time_t t;
	pid_t pid;
	srand ((unsigned) time (&t));
	printf("credito inicial: %d \n", credit);
    pipe(fd); //pipe, antes de criar o filho, para ser o mesmo pipe no pai e no filho
    for(i = 0; i<5;i++){
		printf("ronda %d \n", i);
		pid = fork();
		if(pid==0){//filho
		close(fd[1]); //fecha a escrita
        read(fd[0], &instruction, sizeof(instruction));
        close(fd[0]); //fecha a leitura
			if(instruction==1){
				printf("Aposte! \n");
				scanf("\n %d", &y);
				printf("bet do filho: %d \n", y);
				exit(y);
			} else{
				exit(0);
			}
		}
		else{
        close(fd[0]); //fecha a leitura
		x = rand() % 5;
			if(credit>0){
				instruction = 1;
				write(fd[1], &instruction, sizeof(instruction));
			} else{
				printf("ficou sem dinheiro! \n");
				instruction = 0;
				write(fd[1], &instruction, sizeof(instruction));
			}
        close(fd[1]); //fecha a escrita
        waitpid(pid, &status, 0);
        bet = WEXITSTATUS(status);
			if(bet>0){
				printf("bet do pai: %d \n", x);
				if(bet==x){
					printf("acertou! \n");
					credit = credit + 10;
					printf("crédito: %d \n", credit);
				}else{
					printf("errou! \n");
					credit = credit - 5;
					printf("crédito: %d \n", credit);
				}
			}
        
		}
	}

	
}




int main(){
    pl2_ex10();
    return 0;
}
