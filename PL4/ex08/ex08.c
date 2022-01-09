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

#define SIZE 3

int pl4_ex08() {
    int i, j=0, c=0, pid_saver;
		sem_t *sem;
		sem_t *sem_a;
		/* We create the semaphore */
		if ((sem = sem_open("sema8", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
 					perror("Error in sem_open()");
 				exit(1);
		}

			/* We create the semaphore */
		if ((sem_a = sem_open("sema8_a", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
 					perror("Error in sem_open()");
 				exit(1);
		}


		for(i=0; i<1; i++) {
			pid_saver = fork();
			if(pid_saver == 0) {
				while(j < SIZE) {
					if(c==0) {
						write(1,"C",strlen("C"));
						c++;
						sem_post(sem_a);
					} else {
						c=0;
						sem_wait(sem);
					}
				j++;
				}
				exit(0);
			} else {
				while(j < SIZE) {
				if(c==0) {
						write(1,"S",strlen("S"));
						c++;
						sem_post(sem);
					} else {
						c=0;
						sem_wait(sem_a);
					}
				j++;
			}
			}
		}


		sem_unlink("sema8");
		sem_unlink("sema8_a");

    return 0;
}


int main(void) {
    int value = pl4_ex08();
    return value;
}