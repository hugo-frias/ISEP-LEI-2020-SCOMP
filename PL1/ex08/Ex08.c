#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex08() {
 pid_t p;

 if (fork() == 0) {
 printf("PID = %d\n", getpid()); exit(0);
 }

 if ((p=fork()) == 0) {
 printf("PID = %d\n", getpid()); exit(0);
 }

 printf("Parent PID = %d\n", getpid());

 printf("Waiting... (for PID=%d)\n",p);
 waitpid(p, NULL, 0);

 printf("Enter Loop...\n");
 while (1); /* Infinite loop */
 } 
}

int main(){

	pl1_ex08();
	return 0;
}

Respostas:

a) Não. Os processos filhos não aparecem listados, porque eles foram terminados. Já o pai continua a correr,
por culpa do loop infinito.
b) Ambos os processos são terminados antes que o programa termine.
