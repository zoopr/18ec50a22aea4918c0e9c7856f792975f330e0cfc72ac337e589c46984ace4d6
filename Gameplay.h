//
// Created by Mat on 18/01/30.
//
// Controllo degli step logici all'interno del turno.
//

#ifndef CLUEDO_GAMEPLAY_H
#define CLUEDO_GAMEPLAY_H

#include "Standards.h"
#include "Pieces.h"

void leggiTaccuino(char* filename);
void rollDice(int dice[2]);
void validPaths(int layout[STANZE_N], int val, _Bool out[STANZE_N]);
_Bool checkSolution(const char*stanza,const char* arma,const char* sospetto);
void saveState(char filename[STANDARD_STRLEN], Tabellone* board);

#endif //CLUEDO_GAMEPLAY_H
