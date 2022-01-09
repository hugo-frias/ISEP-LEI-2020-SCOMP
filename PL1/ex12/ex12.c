#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int spawn_childs(int n){
	
	int i,res;
		for(i=0;i<n;i++){
			pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
			if(p==0){
				res= i+1;
				return res;
			}else if(p>0){
				wait(NULL);
			}
		}
	return 0;
}

void pl1_ex12a(){
	int n=6;
	int k =spawn_childs(n);
	printf("%d\n", k);
	
}

void pl1_ex12b(){
	int n=6;
	int k = spawn_childs(n)*2;
	printf("%d\n", k);
}

int main(){
	
	pl1_ex12a();
	pl1_ex12b();
	return 0;
}
