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

#define num_sems 3
#define num_childs 2
#define num_messages 2

int create_childs(int num_filhos)
{
    pid_t pid;
    int i;
    for (i = 0; i < num_filhos; i++)
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

int main()
{
    int i;
    sem_t *sem[num_sems];
    char sems_names[][25] = {"/semaforo_father_ex07", "/semaforo_son1_ex07", "/semaforo_son2_ex07"};
    int semns_init_vales[] = {1, 0, 0};
    enum index
    {
        FATHER,
        SON1,
        SON2
    };

    // Inicializar semáforos
    for (i = 0; i < num_sems; i++)
    {
        if ((sem[i] = sem_open(sems_names[i], O_CREAT | O_EXCL, 0644, semns_init_vales[i])) == SEM_FAILED)
        {
            perror("No sem_open()");
            exit(1);
        }
    }

    // Disable do buffer do stdout de modo a o printf ser executado automáticamente sem ter de esperar por uma nova linha
    setbuf(stdout, NULL);
    
    // Criar processos filho
    int id = create_childs(num_childs); // Pai = 0; Filhos > 0

    // Código do processo pai
    if (id == 0)
    {
        for (i = 0; i < num_messages; i++)
        {
            sem_wait(sem[FATHER]);

            if (i == 0)
            {
                printf("Sistemas ");
            }
            else
            {
                printf("a ");
            }

            sem_post(sem[SON1]);
        }
    }

    // Código do 1º processo filho
    if (id == 1)
    {
        for (i = 0; i < num_messages; i++)
        {
            sem_wait(sem[SON1]);

            if (i == 0)
            {
                printf("de ");
            }
            else
            {
                printf("melhor ");
            }

            sem_post(sem[SON2]);
        }

        exit(0);
    }

    // Código do 2º processo filho
    if (id == 2)
    {
        for (i = 0; i < num_messages; i++)
        {
            sem_wait(sem[SON2]);

            if (i == 0)
            {
                printf("Computadores -");
            }
            else
            {
                printf("disciplina! ");
            }

            sem_post(sem[FATHER]);
        }

        exit(0);
    }

    // Esperar que todos os filhos terminem a sua execução
    for (i = 0; i < num_childs; i++)
    {
        wait(NULL);
    }
    printf("\n");

    // Fechar os semáforos
    sem_close(sem[FATHER]);
    sem_close(sem[SON1]);
    sem_close(sem[SON2]);

    // Remover ficheiros dos semáforos
    sem_unlink(sems_names[FATHER]);
    sem_unlink(sems_names[SON1]);
    sem_unlink(sems_names[SON2]);

    return 0;
}
