#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl2_ex08() {
    int fd[2];  //descritor
    int i,nFilhos = 10, processIDSaver[10];
    struct estrutura {
        char *win;
        int ronda;
    } estrutura; 
    
    pipe(fd); //pipe, antes de criar o filho, para ser o mesmo pipe no pai e no filho
    
    for(i = 0; i<nFilhos;i++){
		processIDSaver[i] = fork();
		if(processIDSaver[i]==0){//filho
		close(fd[1]); //fecha a escrita
        read(fd[0], &estrutura.win, sizeof(estrutura.win)+1); 
        read(fd[0], &estrutura.ronda, sizeof(estrutura.ronda)); 
        printf("O filho leu do pipe o valor: %s\n", estrutura.win); 
        printf("O filho leu do pipe o valor: %d\n", estrutura.ronda);
        close(fd[0]); //fecha a leitura
        exit(estrutura.ronda);
		}
		else{
		char temp[] = "win";
		estrutura.win = temp;
		estrutura.ronda = i;
        close(fd[0]); //fecha a leitura
        write(fd[1], estrutura.win, sizeof(estrutura.win));
        write(fd[1], &estrutura.ronda, sizeof(estrutura.ronda)); 
        close(fd[1]); //fecha a escrita
        sleep(2);
		}
	}
	
	for(i = 0; i<nFilhos; i++){
		
        printf("O filho com o pid %d teve como winning round %d\n", processIDSaver[i], i); 
		
		}
	
}




int main(){
    pl2_ex08();
    return 0;
}
