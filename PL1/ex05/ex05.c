#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex05() {
	int status;
	int i;
	pid_t pid;
	for(i = 1; i<3; i++){
		pid = fork();
	if(pid==0){ //filho
		sleep(i);
		exit(i);
	}
	else{
		printf("Pai espera\n");
		waitpid(pid,&status,0);
		printf("retValue: %d\n",WEXITSTATUS(status));
		
		
		
		
		
		
		
		
		
	}
	}
	printf("paiii");

}

int main(){

	pl1_ex05();
	return 0;
}
