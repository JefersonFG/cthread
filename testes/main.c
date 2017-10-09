#include <stdio.h>
#include "../include/cthread.h"

void* TestFunc(void *arg) {
    printf("Print da TestFunc!\n");
    return;
}

int main() {
    // Programa de teste da biblioteca
    int t1 = -1;

    t1 = ccreate(TestFunc, (void *) NULL, 0);

    printf("O valor de retorno da função ccreate é: %d\n", t1);

    cyield();

    printf("Print da main depois do cyield!\n");

    return 0;
}