#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>

#define shm_name "/shm_ex15"
#define qtd_sems 2
#define num_childs 10
#define show_room_size 2
#define open_time 1
#define close_time 20

typedef struct
{
    int size;
    int visitors;
} show_room;

int create_childs(int qtd_filhos)
{
    pid_t pid;
    int i;
    for (i = 0; i < qtd_filhos; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Erro no fork!\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            return i + 1;
        }
    }

    return 0;
}

void get_in(show_room *room)
{
    room->visitors++;
    printf("Entrei na sala! (PID:%d)\n", getpid());
}

void get_out(show_room *room)
{
    room->visitors--;
    printf("Sai da sala! (PID:%d)\n", getpid());
}

int main()
{
    int i, bool;
    time_t start, now;
    sem_t *sem[qtd_sems];
    char sems_names[][25] = {"/semaforo_shm_ex15", "/semaforo_door_ex15"};
    int semns_init_vales[] = {1, 1};
    enum index
    {
        SHM,
        DOOR
    };
    int fd, data_size = sizeof(show_room);
    show_room *shared_data;
    
    // Inicializar memória partilhada
    fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Erro ao abrir a memoria!\n");
        exit(EXIT_FAILURE);
    }
    
    if (ftruncate(fd, data_size) == -1)
    {
        perror("Erro a definir o tamanho!\n");
        exit(EXIT_FAILURE);
    }

    shared_data = (show_room *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("Erro no mmap!\n");
        exit(EXIT_FAILURE);
    }
    
    // Inicializar valores da estrutura
    shared_data->size = show_room_size;
    shared_data->visitors = 0;

    // Inicializar semáforos
    for (i = 0; i < qtd_sems; i++)
    {
        if ((sem[i] = sem_open(sems_names[i], O_CREAT | O_EXCL, 0644, semns_init_vales[i])) == SEM_FAILED)
        {
            perror("No sem_open()");
            exit(1);
        }
    }
    
    // Iniciar a contagem do tempo
    start = time(NULL);

    // Criar processos filho
    int id = create_childs(num_childs); // Pai = 0; Filhos > 0

    /* Código de todos os processos */
    // Verificar se são horas de poder entrar na sala
    now = time(NULL);
    while ((now - start) < open_time || (now - start) > close_time) //tempo não está entre as horas estipuladas
    {
        printf("Não está na hora de entrar, tenho de esperar.\n");
        sleep(1);
        now = time(NULL);
    }

    // Verificar se a sala está cheia
    bool = 0;
    while (!bool)
    {
        sem_wait(sem[SHM]);
        if (shared_data->visitors == shared_data->size)
        {
            sem_post(sem[SHM]);
            printf("A sala está cheia!\n");
            sleep(1);
        }
        else
        {
            bool = 1;
        }
    }
    
    // Esperar q a porta esteja disponivel para atravessar
    sem_wait(sem[DOOR]);
    // Entrar na sala
    get_in(shared_data);
    printf("Num de pessoas na sala: %d\n", shared_data->visitors);
    sem_post(sem[DOOR]);
    sem_post(sem[SHM]);

    // Esperar dentro da sala
    sleep(1);

    // Verificar se são horas de poder entrar na sala
    now = time(NULL);

	while ((now - start) < open_time || (now - start) > close_time) //tempo não está entre as horas estipuladas
    {
        printf("Não está na hora de sair, tenho de esperar.\n");
        sleep(1);
        now = time(NULL);
    }

    // Esperar q a porta esteja disponivel para atravessar
    sem_wait(sem[SHM]);
    sem_wait(sem[DOOR]);
    // Sair da sala
    get_out(shared_data);
    sem_post(sem[DOOR]);
    sem_post(sem[SHM]);

	if(id > 0){
        printf("Vou para casa! (PID:%d)\n", getpid());
        exit(1);
    }

    // Esperar q todos os filhos morram
    for(i=0; i<num_childs; i++){
        wait(NULL);
    }

    // Fechar a memória partilhada
    if (munmap(shared_data, 100) < 0)
    {
        exit(1);
    }

    if (shm_unlink(shm_name) < 0)
    {
        exit(1);
    }
    
    // Fechar os semáforos
    sem_close(sem[SHM]);
    sem_close(sem[DOOR]);

    // Remover ficheiros dos semáforos
    sem_unlink(sems_names[SHM]);
    sem_unlink(sems_names[DOOR]);

    return 0;
}
