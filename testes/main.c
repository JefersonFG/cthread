#include <stdio.h>
#include "../include/cthread.h"

#define IDENTIFICATION_SIZE 110

void* TestFunc(void *arg) {
    printf("Print da TestFunc!\n");
}

void* TestFunc2(void *arg) {
    printf("Print da TestFunc2!\n");
}

int main() {
    int t1 = -1;
    int t2 = -1;
    char identification[IDENTIFICATION_SIZE];

    t1 = ccreate(TestFunc, (void *) NULL, 0);
    t2 = ccreate(TestFunc2, (void *) NULL, 0);

    printf("Print da função main\n");

    cjoin(t1);
    cjoin(t2);

    printf("Identificação do grupo:\n");
    cidentify(identification, IDENTIFICATION_SIZE);
    puts(identification);

    return 0;
}