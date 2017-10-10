#include <stdio.h>
#include "../include/cthread.h"

#include <unistd.h>

void* TestFunc(void *arg) {
    printf("Print da TestFunc antes do primeiro cyield!\n");

    //sleep(1);

    cyield();

    printf("Print da TestFunc depois do primeiro cyield!\n");

    cyield();

    printf("Print final da TestFunc!\n");
}

int main() {
    // Programa de teste da biblioteca
    int t1 = -1;

    t1 = ccreate(TestFunc, (void *) NULL, 0);

    printf("O valor de retorno da função ccreate é: %d\n", t1);

    printf("Print da main antes do primeiro cyield!\n");

    cyield();

    printf("Print da main depois do primeiro cyield!\n");

    cyield();

    printf("Print da main depois do segundo cyield!\n");

    cyield();

    printf("Print da main depois do terceiro cyield!\n");

    return 0;
}