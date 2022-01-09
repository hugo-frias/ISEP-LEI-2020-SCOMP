#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_OPERATIONS 1000000
#define ANSWER "Negativo, uma vez que não controlamos a ordem de execução do pai e do filho."

typedef struct {
    int var1;
    int var2;
} shared_data_type;


int pl3_ex05() {
    int i, fd, returnValue, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest5", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    /* Simple error handling */
    if (fd < 0) exit(1);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* Simple error handling */
    if (shared_data == NULL) exit(1);

    /* We initialize the variables like asked in the statement */
    shared_data->var1 = 8000;
    shared_data->var2 = 200;

    returnValue = fork();
    /* Simple error handling */
    if (returnValue < 0) exit(1);


    if (returnValue > 0) {
        /* We create integer variables to make just one acess to memory (for each one) */
        int var1 = shared_data->var1;
        int var2 = shared_data->var2;
        for (i = 0; i < NUM_OPERATIONS; i++) {
            var1++;
            var2--;
        }
        /* We update the two integer variables in the shared memory */
        shared_data->var1 = var1;
        shared_data->var2 = var2;
    } else {
        /* We create integer variables to make just one acess to memory (for each one) */
        int var1 = shared_data->var1;
        int var2 = shared_data->var2;
        for (i = 0; i < NUM_OPERATIONS; i++) {
            var1--;
            var2++;
        }
        /* We update the two integer variables in the shared memory */
        shared_data->var1 = var1;
        shared_data->var2 = var2;
        exit(0);
    }

    printf("VAR1: \t %d \n", shared_data->var1);
    printf("VAR2 \t %d \n", shared_data->var2);

    /**
  * Disconnects the shared memory area from the process address space
  */

    returnValue = munmap(shared_data, data_size);
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    /**
    * Closes the file descriptor
    */

    returnValue = close(fd);
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    /**
    * Removes memory area from file system
    * May not be immediate if any process still has it open
    * The area is marked to be removed as soon as all processes close it
    */

    returnValue = shm_unlink("/shmtest5");
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    return 0;

}


int main(void) {
    int value = pl3_ex05();
    printf("\n \t %s \n", ANSWER);
    return value;
}