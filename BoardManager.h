//
// Created by Mat on 18/01/30.
//

#ifndef CLUEDO_BOARDMANAGER_H
#define CLUEDO_BOARDMANAGER_H

#include "Pieces.h"

Giocatore* playerInit(int* num);
Tabellone* FreshStart();
Tabellone* LoadBoard(char* filename);

void MainGame(Tabellone* tavolo);
_Bool Turn(Tabellone* tavolo, Giocatore* giocatore);

#endif //CLUEDO_BOARDMANAGER_H
