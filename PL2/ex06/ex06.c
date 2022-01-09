#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define VEC_SIZE 1000
#define NR_CHILD_PROCESSES 5

int pl2_ex06() {
    time_t t;
	srand ((unsigned) time (&t));
	int status;
	int i;
	int j;
	int total = 0;
	int tmp = 0;
	int vec1[VEC_SIZE];
	int vec2[VEC_SIZE];
	int fd[2];
	int vec3[NR_CHILD_PROCESSES];
	if (pipe(fd) == -1) {
		perror("Erro ao criar pipe");
		return 1;
    }
    
    //Preencher os 2 vetores
    for(i=0;i<VEC_SIZE;i++) {
		*(vec1+i) = rand() % 200;
		*(vec2+i) = rand() % 200;
	}
    
    //Criar 5 processos filhos e fazer contas
    for(i=0;i<NR_CHILD_PROCESSES;i++) {
		*(vec3+i) = fork();
		if (*(vec3+i) == 0) {
			for(j=200*i;j<200*(i+1);j++) {
				tmp += vec1[j] + vec2[j];
			}
			close(fd[0]);
			write(fd[1],&tmp,sizeof(tmp));
			close(fd[1]);
			exit(0);
		}
	}
	
	//Processo pai espera que todos os filhos acabem e soma todas as somas parciais
	for(i=0;i<NR_CHILD_PROCESSES;i++) {
		waitpid(*(vec3+i),&status,0);
	}
	int r = 0;
	close(fd[1]);
	while(read(fd[0],&r,sizeof(r)) > 0) {
		total += r;
	}
	close(fd[0]);
	return total;
}

int main() {
    int i = pl2_ex06();
    printf("Resultado Final: %d", i);
    return 0;
}

a) Não é necessário um mecanismo de sincronização, uma vez que o write não irá bloquear porque o pipe
nunca ficará cheio neste caso, e o read não irá bloquear porque nunca haverá falta de dados para ler, 
porque o processo pai espera que todos os processos filhos terminem 
(e consequentemente, haja conteúdo no pipe para ser lido pelo pai).
