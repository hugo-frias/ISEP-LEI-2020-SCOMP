#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 33

typedef struct
{
    int normalInteger;
    char normalArray[BUFFER_SIZE];
} send_information;


int pl2_ex02() {

    send_information auxiliarStruct;
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }
    pid_t pid = fork();
    if (pid > 0) {
        close(fd[0]);
        printf("Escreva algum Integer: \n");
        scanf("%d", &auxiliarStruct.normalInteger);
        printf("Escreva alguma String (<33 bytes): \n");
        scanf("%s", auxiliarStruct.normalArray);
        write(fd[1], &auxiliarStruct, sizeof(auxiliarStruct));
        close(fd[1]);
    } else if (pid == 0){
        close(fd[1]);
        read(fd[0], &auxiliarStruct, sizeof(auxiliarStruct));
        printf("Inteiro e String: \t %d \t %s \n", auxiliarStruct.normalInteger, auxiliarStruct.normalArray);
        close(fd[0]);
    }
    return 0;
}

int main(void) {
    return pl2_ex02();
    }
