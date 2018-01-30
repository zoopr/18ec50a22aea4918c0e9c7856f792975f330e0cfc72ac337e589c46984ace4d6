//
// Created by Mat on 18/01/30.
//

#ifndef CLUEDO_BOARDMANAGER_H
#define CLUEDO_BOARDMANAGER_H

#include "Pieces.h"

Giocatore* playerInit();
Tabellone* FreshStart();
Tabellone* LoadBoard(char* filename);
void MainGame(Tabellone* tavolo);

#endif //CLUEDO_BOARDMANAGER_H
