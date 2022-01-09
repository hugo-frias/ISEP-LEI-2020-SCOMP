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

#define NUM_IT 100
#define NUM_D 3
#define MAX_ONCE 5
#define SIMULATE_TIME 3

/* Note: We could use one single integer to evaluate all the doors */
/* Note: We don't wait for the next passangers to enter */

typedef struct {
    int doors[NUM_D];
    int maximum_passangers;
    int current_passangers;
    int total_in;
    int total_out;
} shared_data_type;

/* Check if all doors are currently being used */
int fulldoors(int *doors, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (doors[i] == 0) {
            return i;
        }
    }
    return -1;
}

/* Changes one door fron unused to currently used */
int using_door(int *doors, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (doors[i] == 0) {
            doors[i] = 1;
            return i;
        }
    }
    return -1;
}

/* Changes one door fron currently used to unused */
int not_using_door_anymore(int *doors, int door) {
    if (doors[door] == 1) {
        doors[door] = 0;
        return 0;
    }
    return -1;
}

int pl4_ex11() {
    int i, j, aux, door, passangers, control_in, control_out, returnValue, fd, pid_saver[NUM_IT], data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    sem_t *sem;
    /* Creates and opens a shared memory area */
    /* If already exists, just opens to use */
    fd = shm_open("/shmtest11", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    /* Simple error handling */
    if (fd < 0) exit(1);
    /*  Defines the size of the shared memory */
    ftruncate(fd, data_size);
    /* Maps the shared area in the process address space */
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* Simple error handling */
    if (shared_data == NULL) exit(1);

    /* We create the semaphore */
    if ((sem = sem_open("sema11", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    /* We put the correct data on the struct */
    shared_data->maximum_passangers = 200;
    shared_data->current_passangers = 0;
    shared_data->total_in = 0;
    shared_data->total_out = 0;
    /* We initialize all the doors at 0 (unused) */
    for (i = 0; i < NUM_D; i++) {
        shared_data->doors[i] = 0;
    }

    for (i = 0; i < NUM_IT; i++) {
        /* The parent process creates a new child process */
        pid_saver[i] = fork();
        /* Only the child process will enter in this if */
        if (pid_saver[i] == 0) {
            passangers = shared_data->current_passangers;
            control_in = shared_data->total_in;
            control_out = shared_data->total_out;
            /* Enter in the train */
            if (i % 2 == 0) {
                /* The number of passangers to enter the train */
                aux = rand() % MAX_ONCE;
                if (shared_data->current_passangers == shared_data->maximum_passangers ||
                    0 > fulldoors(shared_data->doors, NUM_D)) {
                    /* If all doors are being used and/or the train is full */
                    sem_wait(sem);
                    /* Changes one door fron unused to currently used */
                    door = using_door(shared_data->doors, NUM_D);
                    /* The usual control stuff */
                    if (door < 0) {
                        exit(1);
                    }
                    j = shared_data->current_passangers;
                    /* We add the number of passangers that enter */
                    j += aux;
                    if (j > shared_data->maximum_passangers) {
                        aux = shared_data->maximum_passangers - shared_data->current_passangers;
                        shared_data->total_in += aux;
                        /* We add the number of passangers that entered */
                        shared_data->current_passangers = shared_data->maximum_passangers;
                    } else {
                        /* We add the number of passangers that entered */
                        shared_data->current_passangers = j;
                        shared_data->total_in += aux;
                    }
                    /* Changes one door fron currently used to unused */
                    door = not_using_door_anymore(shared_data->doors, door);
                    sem_post(sem);
                } else {
                    j = shared_data->current_passangers;
                    /* Changes one door from unused to currently used */
                    door = using_door(shared_data->doors, NUM_D);
                    /* The usual control stuff */
                    if (door < 0) {
                        exit(1);
                    }
                    /* We add the number of passangers that entered */
                    j += aux;
                    if (j > shared_data->maximum_passangers) {
                        aux = shared_data->maximum_passangers - shared_data->current_passangers;
                        shared_data->total_in += aux;
                        shared_data->current_passangers = shared_data->maximum_passangers;
                    } else {
                        /* We add the number of passangers that entered */
                        shared_data->current_passangers = j;
                        shared_data->total_in += aux;
                    }
                    door = not_using_door_anymore(shared_data->doors, door);
                }
                printf("PID(%d) Entraram passageiros (%d). \n", getpid(), aux);
                /* The usual control stuff */
                if (door < 0) {
                    shared_data->current_passangers -= aux;
                    shared_data->total_in -= aux;
                    exit(1);
                }

                /* Leave the train */
            } else {
                /* The number of passangers to leave the train */
                aux = rand() % MAX_ONCE;
                if (aux != 0 && 0 > fulldoors(shared_data->doors, NUM_D)) {
                    /* If the number of passangers generated are not null and all doors are being used */
                    sem_wait(sem);
                    /* The usual control stuff */
                    /* Changes one door fron unused to currently used */
                    door = using_door(shared_data->doors, NUM_D);
                    /* The usual control stuff */
                    if (door < 0) {
                        exit(1);
                    }
                    j = shared_data->current_passangers;
                    /* Just to prevent */
                    if (j == 0) {
                        aux = 0;
                    }
                    /* We remove the number of passangers that leave */
                    j -= aux;
                    if (j < 0) {
                        shared_data->current_passangers = 0;
                        aux = j;
                        shared_data->total_out += aux;
                    } else {
                        shared_data->current_passangers -= aux;
                        shared_data->total_out += aux;
                    }
                    door = not_using_door_anymore(shared_data->doors, door);
                    sem_post(sem);
                } else {
                    /* Changes one door fron unused to currently used */
                    door = using_door(shared_data->doors, NUM_D);
                    /* The usual control stuff */
                    if (door < 0) {
                        exit(1);
                    }
                    j = shared_data->current_passangers;
                    if (shared_data->current_passangers == 0) {
                        aux = 0;
                    }
                    /* We remove the number of passangers that leave */
                    j -= aux;
                    if (j < 0) {
                        shared_data->current_passangers = 0;
                        aux = j;
                        shared_data->total_out += aux;
                    } else {
                        shared_data->current_passangers -= aux;
                        shared_data->total_out += aux;
                    }
                    door = not_using_door_anymore(shared_data->doors, door);
                }
                /* Just to prevent */
                if (aux != 0) {
                    printf("PID(%d) Saíram passageiros (%d) \n", getpid(), aux);
                }
                /* The usual control stuff */
                if (door < 0) {
                    shared_data->total_out -= aux;
                    exit(1);
                }
            }
            if (shared_data->current_passangers > 200 || shared_data->current_passangers < 0 ||
                shared_data->total_out + shared_data->current_passangers != shared_data->total_in) {
                shared_data->current_passangers = passangers;
                shared_data->total_in = control_in;
                shared_data->total_out = control_out;
                exit(1);
            }
            sem_post(sem);
            exit(0);
        }
    }

    int status;
    /* Father waits for all his childs */
    for (i = 0; i < NUM_IT; i++) {
        waitpid(pid_saver[i], &status, 0);
    }

    /* The usual control stuff */
    printf("\nPassageiros dentro do comboio:  %d \n", shared_data->current_passangers);
    printf("Total de Entradas:  %d \n", shared_data->total_in);
    printf("Total de Saídas:  %d \n", shared_data->total_out);


    /**
* We unlink the semaphore created before
*/

    sem_unlink("sema11");

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

    returnValue = shm_unlink("/shmtest11");
    /* Simple error handling */
    if (returnValue < 0) exit(1);

    return 0;
}


int main(void) {
    int value = pl4_ex11();
    return value;
}