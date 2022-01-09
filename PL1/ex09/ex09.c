#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void pl1_ex09() {
		int i,k, NFILHOS=10;
		for(i=0; i<NFILHOS;i++){
			pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
			if(p==0){
				for(k=(i*10)+1;k<=(i+1)*10;k++){
					printf("\n%d",k);
				}
				//printf("\n%d",getppid()); - para verificar se nenhum foi adotado
				exit(0);
			}else if (p>0){
				wait(NULL);
			}
		}

}

int main(){

	pl1_ex09();
	return 0;
}
