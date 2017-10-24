#include <stdio.h>
#include "../include/cdata.h"
#include "../include/scheduler.h"
#include "../include/cthread.h"


#define IDENTIFICATION_SIZE 110
#define RESOURCE_QUANTITY 1

csem_t semaforo;

void GroupIdentification();
void RequestSemaphore();

void* TestFunc(void *arg) {
    printf("Thread 1 em execucao\n");
    RequestSemaphore();
    cyield();
    return 0;

}

void* TestFunc2(void *arg) {
    printf("Thread 2 em execucao\n");
    RequestSemaphore();
    cyield();
    return 0;
}

void* TestFunc3(void *arg) {
    printf("Thread 3 em execucao\n");
    RequestSemaphore();
    cyield();
    return 0;
}

int main() {
    int Thread1 = -1;
    int Thread2 = -1;
    int Thread3 = -1;

    //Imprime a identificacao do grupo
    GroupIdentification();

    //Inicializacao do semaforo
    csem_init(&semaforo, RESOURCE_QUANTITY);

    //Criacao das threads
    Thread1 = ccreate(TestFunc, (void *) NULL, 0);
    Thread2 = ccreate(TestFunc2, (void *) NULL, 0);
    Thread3 = ccreate(TestFunc3, (void *) NULL, 0);

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
    if(cwait(&semaforo) == 0){
        if(semaforo.count >= 0){
            printf(" Recurso alocado com sucesso!\n");
            printf(" Recursos disponíveis: %d\n",semaforo.count);
        } else {
            TCB_t* curthread = GetExecutingThread();
            printf(" Não há recursos disponíveis! Thread bloqueada.\n");
            printf(" Recursos: %d\n",semaforo.count);
            printf(" Thread info:\n-TID: %d\n-State: %d\n",curthread->tid,curthread->state);
        }
    } else if((cwait(&semaforo)) == -1)
        printf(" Recurso indisponivel - Erro ao bloquear a thread!\n");
}
