#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/* Number of characters of the array */
#define NUMBER 100

typedef struct {
char var[NUMBER];
} shared_data_type;

/* This function generates a random char (between 'A' and 'Z') */
char generateRandomChar() {
	//srand((unsigned) time(NULL));
	char c = 'A' + (rand() % 26);
	return c;
}

int writer() {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	
	/* Creates and opens a shared memory area */	
	fd = shm_open("/shmtest4", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

	if(fd == -1) {
		return fd;
	}

    /*  Defines the size of the shared memory */
	ftruncate(fd, data_size);
	
	 /* Maps the shared area in the process address space */
	shared_data = (shared_data_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	if(shared_data == NULL) {
		return -1;
	}
	
	/* We write 100 random chars (between 'A' and 'Z') in the shared memory */
	int i;
	for(i=0;i<NUMBER;i++) {
		shared_data->var[i] = generateRandomChar();
	}
	
	return 0;
}

int reader() {
	char c;
	int returnValue;
	double ascii,sum,average;
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	
	/* Opens the shared memory area created in the writer function */	
	fd = shm_open("/shmtest4", O_RDWR, S_IRUSR|S_IWUSR);
	
	if(fd == -1) {
		return fd;
	}

     /* Maps the shared area in the process address space */
	shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	if(shared_data == NULL) {
		return -1;
	}
	
	int i;
	for(i=0;i<NUMBER;i++) {
		/* We "extract" one char from the shared memory area */
		c = shared_data->var[i];
		/* We "extract" the ASCII code of that char*/
		ascii = (double) c;
		/* We calculate the sum of ASCII code from all chars */
		sum += ascii;
		/* We print each char*/
		printf("char nº %d: %c\n", i+1, c);
	}
	/* We calculate the average of the 100 ASCII codes and print with 2 decimal places */
	average = sum/NUMBER;
	printf("Average: %.2f\n", average);
	
    /* Disconnects the shared memory area from the process address space */

    returnValue = munmap(shared_data, data_size);
    if (returnValue < 0) exit(1);

    /* Closes the file descriptor */

    returnValue = close(fd);
    if (returnValue < 0) exit(1);

    /*
    * Removes memory area from file system
    * May not be immediate if any process still has it open
    * The area is marked to be removed as soon as all processes close it
    */

    returnValue = shm_unlink("/shmtest4");
    if (returnValue < 0) exit(1);
	
	return 0;
}

int pl3_ex04() {
	srand((unsigned) time(NULL));
	int x = writer();
	if(x == -1) {
		return x;
	}
	
	int y = reader();
	if( y == -1) {
		return y;
	}
	
    return 0;
}

int main(void) {
	return pl3_ex04();
}
