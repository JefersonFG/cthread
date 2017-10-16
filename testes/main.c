#include <stdio.h>
#include "../include/cthread.h"

#define IDENTIFICATION_SIZE 110

void* TestFunc(void *arg) {
    printf("Print da TestFunc!\n");
}

int main() {
    int t1 = -1;
    char identification[IDENTIFICATION_SIZE];

    t1 = ccreate(TestFunc, (void *) NULL, 0);
    printf("O valor de retorno da função ccreate é: %d\n", t1);

    cyield();

    printf("Identificação do grupo:\n");
    cidentify(identification, IDENTIFICATION_SIZE);
    puts(identification);

    return 0;
}