#include <stdio.h>
#include "../include/cthread.h"

int main() {
    // Programa de teste da biblioteca
    int x = -1;

    x = ccreate(NULL, NULL, 0);

    printf("O valor de retorno da função ccreate é: %d\n", x);

    return 0;
}