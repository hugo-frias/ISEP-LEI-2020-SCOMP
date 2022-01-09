#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 200

typedef struct
{
    int auxiliarVec[BUFFER_SIZE];
} send_information;


int pl2_ex07() {

    int i, j, k, pipeVerify, status, firstVec[1000], secondVec[1000], resultVec[1000], processIDSaver[5], auxiliarPipeIDSaver[2], pipeIDSaver[10];

    for(i=0; i<10; i=i+2) {
        pipeVerify = pipe(auxiliarPipeIDSaver);
        if(pipeVerify == -1) {
            return -1;
        } else {
            pipeIDSaver[i] = auxiliarPipeIDSaver[0];
            pipeIDSaver[i+1] = auxiliarPipeIDSaver[1];
        }
    }

    for(i=0; i<1000; i++) {
        firstVec[i] = rand() % 34;
        secondVec[i] = rand() % 34;
    }

    status = 1;
    for(i=0; i<5; i++) {
        processIDSaver[i] = fork();
        if(processIDSaver[i] == 0) {
            send_information senderStruct;

    // Fecha todos os PIPES desnecessários
            for(k=0; k<10; k++) {
                if(k != status) {
                    close(pipeIDSaver[k]);
                }
            }
            k=0;
            for(j=i*BUFFER_SIZE; j<(i+1)*BUFFER_SIZE; j++) {
                senderStruct.auxiliarVec[k] = firstVec[j] + secondVec[j];
                k++;
            }

            write(pipeIDSaver[status], &senderStruct, sizeof(senderStruct));
            close(pipeIDSaver[status]);
            exit(0);
        }
      status = status + 2;
    }

    for(i=0; i<5; i++) {
        waitpid(processIDSaver[i], &status, 0);
    }

     send_information receiverStruct;


    // Fecha todos os possíveis PIPES de escrita
    for(i=1;i<10;i=i+2) {
      close(pipeIDSaver[i]);
    }

    i=0; status = 0; pipeVerify = 0;
    for(j=0; j<10; j = j + 2) {
        read(pipeIDSaver[j], &receiverStruct, sizeof(receiverStruct));
        close(pipeIDSaver[j]);

        for(k=status; k<(status+1)*BUFFER_SIZE; k++) {
            resultVec[k] = receiverStruct.auxiliarVec[i];
            i++;
        }
        status++;
        i=0;
    }

    for(i=0; i<1000; i++) {
      printf("%d --- %d \n", i, resultVec[i]);
    }

    return 0;

}

int main(void) {
    return pl2_ex07();
    }
