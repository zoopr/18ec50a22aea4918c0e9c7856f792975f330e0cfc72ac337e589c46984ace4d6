//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <stdio.h>
#include "Gameplay.h"

#define D_SIDES 6 //giusto in caso si voglia giocare con dadi diversi.

void leggiTaccuino(char* filename){
    //todo

}

void scriviTaccuino(char* filename){
    //todo
}

void rollDice(int dice[2]){
    dice[0] = rand()%D_SIDES+1;
    dice[1] = rand()%D_SIDES+1;
    printf("Hai fatto %d + %d", dice[0], dice[1]);
}


void validPaths(int layout[STANZE_N], int val, _Bool out[STANZE_N]){
    int i;
    for (i=0; i<STANZE_N; i++){
        if (layout[i]<= val)
            out[i] = 1;
        else
            out[i] = 0;
    }
}

_Bool checkSolution(const char* stanza,const char* arma,const char* sospetto){
//TODO
}
