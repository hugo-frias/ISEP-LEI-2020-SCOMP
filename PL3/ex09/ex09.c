#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int initialized_vector[1000];
    int maximum_vector[10];
} shared_data_type;


int pl3_ex09() {
    int i, j, returnValue, status, fd, data_size = sizeof(shared_data_type);
    int pid_saver[10];
    shared_data_type *shared_data;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest9", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    /* Simple error handling */
    if (fd < 0) exit(1);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* Simple error handling */
    if (shared_data == NULL) exit(1);

    /**
    * We fullfield the initialized_vector with some random integers
    */

    for (i = 0; i < 1000; i++) {
        shared_data->initialized_vector[i] = rand() % 1000;
    }


    /**
    * Father creates 10 childs
    */

    for (i = 0; i < 10; i++) {
        pid_saver[i] = fork();
        if (pid_saver[i] < 0) exit(1);

            /**
            * Each child calculates the local maximum value
            */

        else if (pid_saver[i] == 0) {
            int maximum = 0;
            for (j = i * 100; j <= (i + 1) * 100; j++) {
                if (shared_data->initialized_vector[j] > maximum) {
                    maximum = shared_data->initialized_vector[j];
                }
            }
            shared_data->maximum_vector[i] = maximum;

            /**
            * We kill the child after all operations to make sure that only father creates new processes
            */

            exit(0);
        }
    }

    /**
    * Father waits for all childs
    */

    for (i = 0; i < 10; i++) {
        j = waitpid(pid_saver[i], &status, 0);
    }

    /**
   * Prints array with all maximum values
   */

    j = 0;
    for (i = 0; i < 10; i++) {
        if (shared_data->maximum_vector[i] > j) {
            j = shared_data->maximum_vector[i];
        }
    }

    printf("Maximum value of the initial array (global maximum): %d \n", j);


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

    returnValue = shm_unlink("/shmtest9");
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    return 0;
}


int main(void) {
    int value = pl3_ex09();
    return value;
}