#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define NR_CHILD_PROCESSES 5
#define NR_PIPES 6


int pl1_ex11() {
  int max_value=0;
  int value;
  int aux;
  int pipes[NR_PIPES*2];
  int process_id;
  pid_t p;
  int i;
  int status;
  int j;
  int pids[NR_CHILD_PROCESSES];
  
 

  //Create 6 pipes
  int fd[2];
  int fd1[2];
  for(i=0;i<NR_PIPES*2;i=i+2) {
    if(pipe(fd) == -1) {
      perror("Erro ao criar pipe!");
      return 1;
    }
    pipes[i] = fd[0];
    pipes[i+1] = fd[1];
  }
  //Create 5 child processes
  process_id = 0;
  p = getpid();
  for(i=0;i<NR_CHILD_PROCESSES;i++) {
    if(p > 0) {
      p = fork();
      if (p == 0) {
        if (process_id == 0) {
          process_id = i+1;
        }
      } 
      else {
        pids[i] = p;
      }
    }
  }

  //random number generator
  srand(getpid());
  value = rand() % 501;
  printf("\n%d ID e numero gerado: %d\n", process_id,value);

  if(process_id==0){//pai
	 
	// pipe para ler  
	fd[0]=pipes[process_id];
	fd[1]=pipes[process_id+1];
	
	// pipe para escrever
	fd1[0] = pipes[process_id+10];
	fd1[1] = pipes[process_id+11];
	
	
    close(fd[0]);
    write(fd[1],&value,sizeof(value));
    close(fd[1]);

    close(fd1[1]);
    read(fd1[0],&max_value,sizeof(max_value));
    printf("\nGreatest random number: %d", max_value);
    close(fd1[0]);

  }else{// filhos
	// pipe para ler
	fd[0]=pipes[process_id*2-2];
	fd[1]=pipes[process_id*2-1];
	
	// pipe para escrever
	fd1[0] = pipes[process_id*2];
	fd1[1] = pipes[(process_id*2)+1];
	
    close(fd[1]);
    read(fd[0],&aux,sizeof(aux));
    
    if(value>aux){
      max_value=value;
    }else{
      max_value=aux;
    }
    
    close(fd[0]);

    
    close(fd1[0]);
    write(fd1[1],&max_value,sizeof(max_value));
    close(fd1[1]);
  }
  
	
	//Kill the remaining child processes
	for(j=0;j<NR_CHILD_PROCESSES;j++) {
		waitpid(pids[j],&status,0);
	}
	return 0;
}

int main(){

	pl1_ex11();
	return 0;
}
