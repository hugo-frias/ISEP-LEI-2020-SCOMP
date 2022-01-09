#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define EACH_CHILD_WORK 5000

typedef struct {
    int customer_code;
    int product_code;
    int quantity;
} product;


int pl2_ex09() {

  product sales[50000];

    int i, j, k, pipeVerify, status, processIDSaver[10], auxiliarPipeIDSaver[2], pipeIDSaver[20];

    // Criamos 10 PIPES
    for(i=0; i<20; i=i+2) {
        pipeVerify = pipe(auxiliarPipeIDSaver);
        if(pipeVerify == -1) {
            return -1;
        } else {
            pipeIDSaver[i] = auxiliarPipeIDSaver[0];
            pipeIDSaver[i+1] = auxiliarPipeIDSaver[1];
        }
    }

    // Preenchemos o array primário sales[50000]
    for(i=0; i<5000; i++) {
        sales[i].product_code = (rand() % 100) + 7;
        sales[i].quantity = (rand() % 150) + 3;
        sales[i].customer_code = (rand() % 50) + 11;
    }

    // Criamos 10 filhos
    // Trabalhamos com eles
    status = 1;
    for(i=0; i<10; i++) {
        processIDSaver[i] = fork();
        if(processIDSaver[i] == 0) {
            int sendInfomation [5000];
            int sendInformationController = 0;

            for(k=0; k<20; k++) {
                if(k != status) {
                    close(pipeIDSaver[k]);
                }
            }
            k=0;
            for(j=i*EACH_CHILD_WORK; j<(i+1)*EACH_CHILD_WORK; j++) {
                if(sales[j].quantity > 20) {
                  sendInfomation[sendInformationController] = sales[j].product_code;
                  sendInformationController++;
                }
            }

            write(pipeIDSaver[status], &sendInfomation, sizeof(sendInfomation));
            close(pipeIDSaver[status]);
            exit(0);
        }
      status = status + 2;
    }

    for(i=0; i<10; i++) {
        waitpid(processIDSaver[i], &status, 0);
    }

     int receiveInformation [5000];
     int product[50000];


    // Fecha todos os possíveis PIPES de escrita
    for(i=1;i<20;i=i+2) {
      close(pipeIDSaver[i]);
    }

    i=0; status = 0; pipeVerify = 0;
    for(j=0; j<20; j = j + 2) {
        read(pipeIDSaver[j], &receiveInformation, sizeof(receiveInformation));
        close(pipeIDSaver[j]);

        for(k=status; k<(status+1)*EACH_CHILD_WORK; k++) {
            product[k] = receiveInformation[i];
            i++;
        }
        status++;
        i=0;
    }

    for(i=0; i<50000; i++) {
      if(product[i] > 0) {
        printf("Product code: \t %d \n", product[i]);
      }
      
    }

    return 0;

}

int main(void) {
    return pl2_ex09();
}
