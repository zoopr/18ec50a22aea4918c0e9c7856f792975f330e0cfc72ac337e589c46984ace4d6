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
void validPaths(const int layout[STANZE_N], int val, _Bool out[STANZE_N]);
int checkSolution(const char*stanza,const char* arma,const char* sospetto, Tabellone* tavolo);
int checkCard(const char* stanza,const char* arma,const char* sospetto, Carta* card);
void saveState(char* filename, Tabellone* board);
void scriviTaccuino(char* filename, char* message);
void logger(char* message);
void statInit(Tabellone* tavolo);
void statSave(Tabellone* tavolo);
void statTrack(Tabellone* tavolo);
void statShow();
int checkCard_Archive(char* (*func)(int, char*), Carta* card, int len);

#endif //CLUEDO_GAMEPLAY_H
