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
    int var1;
    char var2[100];
} shared_data_type;


int writer() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest1", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
/* Simple error handling */
    if (fd < 0) exit(1);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
/* Simple error handling */
    if (shared_data == NULL) exit(1);
    printf("Introduza algum nome \n");
    scanf("%s", shared_data->var2);
    printf("Introduza algum número \n");
    scanf("%d", &shared_data->var1);
    return 0;
}

int reader() {
    int fd, returnValue, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    /* Without O_CREAT tries to use existing (error if none) */
    fd = shm_open("/shmtest1", O_RDWR, S_IRUSR | S_IWUSR);
    /* Simple error handling */
    if (fd < 0) exit(1);
    /* Doesn't need ftruncate, cause we don't need the change the size of shared memory */
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
/* Simple error handling */
    if (shared_data == NULL) exit(1);
    printf("%d \n", shared_data->var1);
    printf("%s \n", shared_data->var2);

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

    returnValue = shm_unlink("/shmtest1");
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    return 0;
}

int pl3_ex01() {

    /**
     *  Devemos executar primeiramente o programa com o reader comentado.
     *  E posteriormente com o writer comentado.
     */

    int x = writer();
    if (x == -1) {
        return x;
    }
    // int y = reader();
    // if( y == -1) {
    // return y;
    // }
    return 0;
}


int main(void) {
    return pl3_ex01();
}