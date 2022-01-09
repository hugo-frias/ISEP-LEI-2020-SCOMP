#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define SIZE_STRUCT 4000

typedef struct {
    char initialized_vector[SIZE_STRUCT];
} shared_data_type;


int pl4_ex03() {
    int returnValue, fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    sem_t *sem;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest3", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    /* Simple error handling */
    if (fd < 0) exit(1);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* Simple error handling */
    if (shared_data == NULL) exit(1);

    /* We create the semaphore */
    if ((sem = sem_open("sema3", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    /* Function learned online with some indian guy */
    sprintf(shared_data->initialized_vector, "%s %d", "I'm the father: ", getpid());
    printf("PID do Pai: %d \n", getpid());

    pid_t pid = fork();

    if(pid == 0) {
        /* Child tries to decrement the semaphore */
        sem_wait(sem);
        sprintf(shared_data->initialized_vector, "%s %d", "I'm the child: ", getpid());
        printf("%s \n",shared_data->initialized_vector);
        /* Child increments the sempahore */
        sem_post(sem);
        exit(0);
    }

    /* Desmark the next line to child print first than the father */
    /* wait(NULL); */
    /* Father tries to decrement the semaphore */
    sem_wait(sem);
    sprintf(shared_data->initialized_vector, "%s %d", "I'm the father: ", getpid()
    );
    /* Father increments the sempahore */
    sem_post(sem);
    /* The next line is always printed by the father */
    printf("%s \n",shared_data->initialized_vector);

    sem_unlink("sema3");

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

    returnValue = shm_unlink("/shmtest3");
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    return 0;
}


int main(void) {
    int value = pl4_ex03();
    return value;
}