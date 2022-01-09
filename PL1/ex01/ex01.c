#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void pl1_ex01() {
    int x = 1;
    pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
    if (p == 0) { // Filho
        x = x+1;
        printf("1. x = %d\n", x);
    } else { // Pai
        x = x-1;
        printf("2. x = %d\n", x);
    }
    printf("3. %d; x = %d\n", p, x);
}

int main(){
    pl1_ex01();
    return 0;
}
