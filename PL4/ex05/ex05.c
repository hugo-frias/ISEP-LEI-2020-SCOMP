#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int pl4_ex05() {
	pid_t pid;
	sem_t *sem;
	int returned_value;
	
	/* We create the semaphore */
	if ((sem = sem_open("sema", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
    /* We create the child process */
	pid = fork();
	
	if (pid > 0) {
		/* Parent decrements the semaphore. */
		/* If reaches first, the process is blocked until the child increments the semaphore */
		sem_wait(sem);
		printf("I'm the father\n");
	} else {
		printf("I'm the child\n");
		/* Child increments the semaphore */
		sem_post(sem);
		exit(0);
	}
	
	/* Removes the semaphore */
	returned_value = sem_unlink("sema");
	if(returned_value < 0) {
		exit(1);
	}
	return 0;
	
}

int main(void) {
	return pl4_ex05();
}
