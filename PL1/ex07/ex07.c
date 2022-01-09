#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex07() {
int numbers[ARRAY_SIZE]; /* array to lookup */
int n; /* the number to find */
time_t t; /* needed to init. the random number generator (RNG)
*/
int i;

/* intializes RNG (srand():stdlib.h; time(): time.h) */
srand ((unsigned) time (&t));

/* initialize array with random numbers (rand(): stdlib.h) */
for (i = 0; i < ARRAY_SIZE; i++)
 numbers[i] = rand () % 1000;

 /* initialize n */
 n = rand () % 1000;
 int total = 0;
 pid_t pid;
 int status;
 pid = fork();
 if(pid != 0){ //pai
	 
	 for(i = 0; i< ARRAY_SIZE/2; i++){
		 if(numbers[i] == n){
			 
			 total++;
		 }
	 }
	 printf("\n pai: %d", total);
	 waitpid(pid, &status,0);
 } else{ //filho
	 	 for(i = ARRAY_SIZE/2; i< ARRAY_SIZE; i++){
		 if(numbers[i] == n){
			 total++;
		 }
	 }
	 printf("\n filho: %d", total);
	 
	 exit(total);
	 }
	 total = total + WEXITSTATUS(status);
	 printf("\n total: %d", total);
	   
 
}

int main(){

	pl1_ex07();
	return 0;
}
