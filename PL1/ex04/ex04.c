#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void pl1_ex04() {
    int a=0, b, c, d;
    b = (int) fork();
    c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
    d = (int) getppid();
    a = a + 5;
    //if(b>0) wait(NULL);
    printf("\na=%d, b=%d, c=%d, d=%d\n", a, b, c, d);
}


int main(){
	pl1_ex04();
    return 0;
}
