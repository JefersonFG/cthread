#include <stdio.h>
#include "../include/cthread.h"


#define IDENTIFICATION_SIZE 110
#define RESOURCE_QUANTITY 1

csem_t semaforo;

void GroupIdentification();
void RequestSemaphore();

void* TestFunc(void *arg) {
    printf("Thread 1 em execucao\n");
    RequestSemaphore();

    return 0;

}

void* TestFunc2(void *arg) {
    printf("Thread 2 em execucao\n");
    RequestSemaphore();

    return 0;
}

void* TestFunc3(void *arg) {
    printf("Thread 3 em execucao\n");
    RequestSemaphore();

    return 0;
}

int main() {
    int Thread1 = -1;
    int Thread2 = -1;
    int Thread3 = -1;

    //Imprime a identificacao do grupo
    GroupIdentification();

    //Criacao das threads
    Thread1 = ccreate(TestFunc, (void *) NULL, 0);
    Thread2 = ccreate(TestFunc2, (void *) NULL, 0);
    Thread3 = ccreate(TestFunc3, (void *) NULL, 0);

    //Inicializacao do semaforo
    csem_init(&semaforo, RESOURCE_QUANTITY);

    cjoin(Thread1);
    cjoin(Thread2);
    cjoin(Thread3);

    return 0;
}

void GroupIdentification() {
    char identification[IDENTIFICATION_SIZE];
    printf("Identificacao do grupo:\n");
    cidentify(identification, IDENTIFICATION_SIZE);
    puts(identification);

}

void RequestSemaphore() {
    printf(" Chamada para cwait\n");
    if(cwait(&semaforo) == 0)
        printf(" Recurso alocado com sucesso!\n");
     if((cwait(&semaforo)) == -1)
        printf(" Recurso indisponivel - Thread bloqueada!\n");
    cyield();

}
