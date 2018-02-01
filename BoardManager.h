//
// Created by Mat on 18/01/30.
//
// Interfaccia tra l'inizializazione e il gameplay. Controllo delle win conditions.
// Control flow principale del gioco in Turn e Turn_AI.
// Le funzioni sono commentate lungo il sorgente.
//

#ifndef CLUEDO_BOARDMANAGER_H
#define CLUEDO_BOARDMANAGER_H

#include "Pieces.h"

Giocatore* playerInit(int* num);
Tabellone* FreshStart();
Tabellone* LoadBoard(char* filename);

void MainGame(Tabellone* tavolo, _Bool(*turnType)(Tabellone*, Giocatore*), _Bool AI);
_Bool Turn(Tabellone* tavolo, Giocatore* giocatore);
_Bool Turn_AI(Tabellone* tavolo, Giocatore* giocatore);

#endif //CLUEDO_BOARDMANAGER_H
