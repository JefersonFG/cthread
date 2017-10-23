#include <stdio.h>
#include "../include/cthread.h"


#define IDENTIFICATION_SIZE 110
#define RESOURCE_QUANTITY 4

csem_t semaforo;

void GroupIdentification();
void RequestSemaphore();
void FreeResource();


void* TestFunc(void *arg) {
    printf("Thread 1 em execucao\n");
    RequestSemaphore();
    FreeResource();
    cyield();
    printf("Thread 1 termina sua execucao\n");

    return 0;

}

void* TestFunc2(void *arg) {
    printf("Thread 2 em execucao\n");
    FreeResource();
    printf("Thread 2 termina sua execucao\n");

    return 0;
}

int main() {
    int Thread1 = -1;
    int Thread2 = -1;

    printf("\nGrupo de testes de do semaforo\n");
    printf("Espera-se que a thread1 entre em execucao e execute a cwait alocando um recurso e logo em seguida libere esse recurso.");
    printf("Em seguida a thread2 entra em execucao e tenta liberar recurso.\n");

    printf("-------Pressione uma tecla para iniciar os testes!-------\n");

    if(getchar()){
    //Criacao das threads
    Thread1 = ccreate(TestFunc, (void *) NULL, 0);
    Thread2 = ccreate(TestFunc2, (void *) NULL, 0);

    //Inicializacao do semaforo
    csem_init(&semaforo, RESOURCE_QUANTITY);

    printf("Recurso inicial do Semaforo: %d\n", semaforo.count);

    cjoin(Thread1);
    cjoin(Thread2);
    }

    return 0;
}

void GroupIdentification() {
    char identification[IDENTIFICATION_SIZE];
    printf("Identificacao do grupo:\n");
    cidentify(identification, IDENTIFICATION_SIZE);
    puts(identification);

}

void RequestSemaphore() {
    if(cwait(&semaforo) == 0)
        printf(" cwait executada com sucesso!\n");
     else
        printf(" cwait executada com erro!\n");

    printf(" Recurso atual: %d\n", semaforo.count);
}

void FreeResource() {
    if(csignal(&semaforo) == 0)
        printf(" csignal executada com sucesso, recurso liberado!\n");
        else if(csignal(&semaforo) == -1)
        printf(" csignal executada com erro! Nao foi possivel bloquear a thread\n");
           else if(csignal(&semaforo) == -2)
            printf(" csignal executada com erro na estrutura de fila!\n");

    printf(" Recurso atual: %d\n", semaforo.count);
}
