#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define NR_PIECES 1000
#define NR_PIPES 5
#define NR_CHILD_PROCESSES 4
#define NR_WORK_PIECES_M1 5
#define NR_WORK_PIECES_M2 5
#define NR_WORK_PIECES_M3 10
#define NR_WORK_PIECES_M4 100

int machine_work(int process_id,int constant,int *pipes,char *message) {
	int piece_to_send;
	int piece_received;
	int index = 2*process_id - 2;
	int fd[2];
	int i;
	int j;
	int fd1[2];
	fd[0] = pipes[index];
	fd[1] = pipes[index+1];
	fd1[0] = pipes[index+2];
	fd1[1] = pipes[index+3];
		
	close(fd[1]);
	close(fd1[0]);
	for(i=0;i<(NR_PIECES/constant);i++) {
		for(j=0;j<constant;j++) {					
		    read(fd[0],&piece_received,sizeof(int));
		    piece_to_send = piece_received;
            write(fd1[1],&piece_to_send,sizeof(int));
		}
		printf("%s\n", message);
	}
	close(fd[0]);
	close(fd1[1]);
	exit(0);
}

int pl2_ex13() {
	//Variables
	int status;
	int old_pieces[NR_PIECES];
	int i;
	int pipes[NR_PIPES*2];
	int process_id;
	pid_t p;
	int pids[NR_CHILD_PROCESSES];
	int piece_received;
	//int piece_to_send;
	int j;
	//int k;
	int fd1[2];
	int inventory[NR_PIECES];
	
	//Fill the old pieces's array
	for(i=0;i<NR_PIECES;i++) {
		old_pieces[i] = i+1;
	}
	
	//Create 5 pipes
	int fd[2];
	for(i=0;i<NR_PIPES*2;i=i+2) {
		if(pipe(fd) == -1) {
			perror("Erro ao criar pipe!");
			return 1;
		}
		pipes[i] = fd[0];
		pipes[i+1] = fd[1];
	}
	
	//Create 4 child processes
	process_id = 0;
	p = getpid();
	for(i=0;i<NR_CHILD_PROCESSES;i++) {
		if(p > 0) {
			p = fork();
			if (p == 0) {
				if (process_id == 0) {
					process_id = i+1;
				}
			} else {
				pids[i] = p;
			}
		}
	}
	
	//Parent send 5 pieces at a time to M1 and receives the new pieces from M4
	if(process_id == 0) {	
		fd[0] = pipes[process_id];
		fd[1] = pipes[process_id+1];
		fd1[0] = pipes[process_id+8];
		fd1[1] = pipes[process_id+9];
		
		close(fd[0]);
		for(j=0;j<NR_PIECES;j++) {	
		    write(fd[1],&old_pieces[j],sizeof(int));
		}
		close(fd[1]);
		
		close(fd1[1]);
		printf("Inventory:");
		for(j=0;j<NR_PIECES;j++) {
			read(fd1[0],&piece_received,sizeof(int));
			inventory[j] = piece_received;
			printf("Piece nr: %d\n", inventory[j]);
		}
		close(fd1[0]);
	}
	
	//Machine M1 takes care of 5 pieces at a time
	if(process_id == 1) {
		machine_work(process_id,NR_WORK_PIECES_M1,pipes,"M1 cutted 5 pieces");
	}
	
	//Machine M2 takes care of 5 pieces at a time
	if(process_id == 2) {
		machine_work(process_id,NR_WORK_PIECES_M2,pipes,"M2 folded 5 pieces");
	}
	
	//Machine M3 takes care of 10 pieces at a time
	if(process_id == 3) {
		machine_work(process_id,NR_WORK_PIECES_M3,pipes,"M3 welded 10 pieces");
	}
	
	//Machine M4 takes care of 100 pieces at a time
	if(process_id == 4) {
		machine_work(process_id,NR_WORK_PIECES_M4,pipes,"M4 packed 100 pieces");
	}
	
	//Kill the remaining child processes
	for(j=0;j<NR_CHILD_PROCESSES;j++) {
		waitpid(pids[j],&status,0);
	}
	return 0;
}

int main() {
    int i = pl2_ex13();
    if (i == 0) {
        printf("\n\nSuccessful Operation!");
    } else {
        printf("\n\nNon Successful Operation!");
    }
    return 0;
}
