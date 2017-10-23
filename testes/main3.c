#include <stdio.h>
#include "../include/cthread.h"

int cont = 1000;

void* TestFunc(void *arg) {
    printf("Thread 1 em execucao\n");
    while(cont > 0){
        cont--;
    }
    cyield();
    printf("Thread 1 termina sua execucao\n");

    return 0;

}

void* TestFunc2(void *arg) {
    printf("Thread 2 em execucao\n");
    while(2*cont > 0){
        cont--;
    }
    cyield();
    printf("Thread 2 termina sua execucao\n");

    return 0;
}

void* TestFunc3(void *arg) {
    printf("Thread 3 em execucao\n");
    while(3*cont > 0){
        cont--;
    }
    cyield();
    printf("Thread 3 termina sua execucao\n");

    return 0;
}


int main() {
    int Thread1 = -1;
    int Thread2 = -1;
    int Thread3 = -1;

    printf("\nGrupo de testes da cjoin e cyield\n");
    printf("Espera-se que a thread1 entre em execucao troque para thread2 e em seguida para a thread3.\n");
    printf("E baseado no tempo de execucao delas o escalonador decide qual executa (e consequentemente quem termina) primeiro.\n");

    printf("-------Pressione uma tecla para iniciar os testes!-------\n");

    if(getchar()){
    //Criacao das threads
    Thread1 = ccreate(TestFunc, (void *) NULL, 0);
    Thread2 = ccreate(TestFunc2, (void *) NULL, 0);
    Thread3 = ccreate(TestFunc3, (void *) NULL, 0);

    cjoin(Thread1);
    cjoin(Thread2);
    cjoin(Thread3);

    }

    return 0;

}

