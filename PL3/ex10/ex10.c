#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct {
int var1;
char var2[100];
} shared_data_type;


int writer() {
int fd, data_size = sizeof(shared_data_type);
shared_data_type *shared_data;
fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

if(fd == -1) {
	return fd;
}

ftruncate(fd, data_size);
shared_data = (shared_data_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

if(shared_data == NULL) {
	return -1;
}

printf("Introduza algum nome \n");
scanf("%s", shared_data->var2);

printf("Introduza algum número \n");
scanf("%d", &shared_data->var1);


return 0;
}



int reader() {
int fd, data_size = sizeof(shared_data_type);
shared_data_type *shared_data;
fd = shm_open("/shmtest", O_RDWR, S_IRUSR|S_IWUSR);
if(fd == -1) {
	return fd;
}

//ftruncate (fd, data_size);
shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

if(shared_data == NULL) {
	return -1;
}

printf("%d \n", shared_data->var1);
printf("%s \n", shared_data->var2);
return 0;
}

int pl3_ex01() {
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
return pl3_ex01();
}
