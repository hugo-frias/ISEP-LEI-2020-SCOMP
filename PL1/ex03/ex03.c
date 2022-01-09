#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex03() {
    fork(); 
    fork(); 
    fork();
    printf("SCOMP!\n");
}


int main(){
	pl1_ex03();
   return 0;
}
