//
// MANGUAÇA
// --------
// Teste para ver o que acontece quando as threads exageram na mardita.
// Valha-me Deus.
//
// Created by vcsoares on 23/10/17.
//

#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

#define GARRAFAS 3
#define BEBUNS 5

time_t endtime;

csem_t ypioca;
csem_t mutex;

int bebendo = 0;

void sleepao() {
    int i = 0;

    i = rand()%5 + 1;
    for (; i<0; i--) cyield();
}

void* beber(void* arg) {
    while (time(NULL) < endtime) {
        cwait(&mutex);
        if (bebendo < GARRAFAS) {
            ++bebendo;
            printf("- AÍ MEU ABENÇOADO! desce mais uma caninha...\n");
            printf("[ tem %d bebuns nesta bodega ]", bebendo);
            cwait(&ypioca);
            csignal(&mutex);
        } else {
            printf("- CADE A CACHAÇA NESSA ESPELUNCA??");
            csignal(&mutex);
        }
        sleepao();
    }
}

void* mais_pinga(void* arg) {
    while (time(NULL) < endtimes)
}