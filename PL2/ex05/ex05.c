#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// source:
// https://www.tutorialspoint.com/c_standard_library/c_function_toupper.htm
void upper_string(char s[0]){
	int i;

    for(i=0;i<strlen(s);i++){
        s[i] = toupper(s[i]);
    }

    //printf("%s", s);
}

int pl2_ex05(){
  char string[256];
  char string1[256];
  //pid_t p;

  //create 2 pipes
    int fd[2];
	int fd1[2];
	if (pipe(fd) == -1) {
		perror("Erro ao criar pipe!");
		return 1;
	}
    if (pipe(fd1) == -1) {
        perror("Erro ao criar pipe!");
        return 1;
    }

  //p = fork();

  if(fork()==0){ //Child

    close(fd[1]);
    close(fd1[0]);
    printf("String: ");
    fflush(stdin);
    scanf("%s", string);
    fflush(stdin);
  	
    write(fd1[1],string,sizeof(string));
    close(fd1[1]);
    
    read(fd[0],string1,sizeof(string1));
    printf("\nString in upper case: %s", string1);
    close(fd[0]);
    exit(0);

  }else{

    close(fd[0]);
    close(fd1[1]);
    read(fd1[0],string1,sizeof(string1));
    close(fd1[0]);
    
    upper_string(string1);
    
    write(fd[1],string1, sizeof(string1));
    close(fd[1]);

  }
  wait(NULL);
  return 0;

}


int main() {
    int i;
    i = pl2_ex05();
    if (i == 1) {
        printf("\n\nOperação não concluída!");
    }else{
        printf("\n\nOperação concluída!");
    }
    return 0;
}
