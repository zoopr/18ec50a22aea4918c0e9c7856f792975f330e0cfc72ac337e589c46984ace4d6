//
// Created by Mat on 18/01/31.
//
// Logica decisionale AI.
//



#ifndef CLUEDO_AI_H
#define CLUEDO_AI_H

#include "Pieces.h"

void printTableStatus (Tabellone* tavolo);
void initInterest(Tabellone* tavolo, float loadArea[3][STANZE_N]);
void saveInterest(Tabellone* tavolo, float loadArea[3][STANZE_N]);
void readInterest(Tabellone* tavolo, float loadArea[3][STANZE_N]);
void generateCoordinates(Carta* carta, int coords[2]);
int movementStrategy(const float interesseStanze[STANZE_N],const _Bool reachable[STANZE_N]);
int suspectStrategy(const float interesse_AoS[ARMI_N]);
#endif //CLUEDO_AI_H
