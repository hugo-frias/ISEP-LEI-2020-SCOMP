~#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define STRING_ONE_SIZE 80
#define STRING_TWO_SIZE 80

int pl2_ex03() {
	int status;
	char write_one[STRING_ONE_SIZE] = "Hello world!";
	char write_two[STRING_TWO_SIZE] = "Goodbye!";
	char read_one[STRING_ONE_SIZE] = {0};
	char read_two[STRING_ONE_SIZE] = {0};
	pid_t p;
	int fd[2];
	if (pipe(fd) == -1) {
		perror("Erro ao criar pipe");
		return 1;
    }
    
	p = fork();
	if (p > 0) {
		//Processo pai escreve mensagens no pipe e espera que o filho as leia
		close(fd[0]);
		write(fd[1],write_one,strlen(write_one));
		write(fd[1],write_two,strlen(write_two));
		close(fd[1]);
		
		wait(&status);
		
		printf("\n\nPid do filho: %d", p);
		printf("\nValor de saída do processo filho: %d", WEXITSTATUS(status));
	} else {
	   if (p == 0) {
		   //Processo filho lê as mensagens que se encontram no pipe e imprime-as
		   close(fd[1]);
		   read(fd[0],read_one,strlen(write_one));
		   printf("Primeira mensagem: %s", read_one);
		   read(fd[0],read_two,strlen(write_two));
		   printf("\nSegunda mensagem: %s", read_two);
		   close(fd[0]);
		   exit(0);
	   }
    }
    return 0;
}

int main() {
    int i = pl2_ex03();
    if (i == 0) {
        printf("\n\nOperação bem sucedida!");
    } else {
        printf("\n\nOperação má sucedida!");
    }
    return 0;
}
