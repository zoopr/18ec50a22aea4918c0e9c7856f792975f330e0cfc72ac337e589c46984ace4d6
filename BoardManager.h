//
// Created by Mat on 18/01/30.
//
// Interfaccia tra l'inizializazione e il gameplay. Controllo delle win conditions.
// Control flow principale del gioco in Turn e Turn_AI.
// L'acquisizione dati dall'utente è gestita tramite strtol, che permette di sanitizzare l'input non-numerico settandolo a 0.
// I menu sono presentati all'utente a partire da 1, sia per convenienza che per la gestione dell'eccezione appena accennata.
// Le funzioni sono commentate lungo il sorgente.
//
// NOMI GIOCATORE
// Da specifiche, la modalità AI non richiede di inserire nomi per i singoli giocatori.
// Se si vuole usare nomi personalizzati, si può caricare in completa compatibilità un file generato nella modalità classica.
//

#ifndef CLUEDO_BOARDMANAGER_H
#define CLUEDO_BOARDMANAGER_H

#include "Pieces.h"

Giocatore* playerInit(int* num, _Bool AI);
Tabellone* FreshStart(_Bool AI);
Tabellone* LoadBoard(char* filename);
void MainGame(Tabellone* tavolo, _Bool(*turnType)(Tabellone*, Giocatore*), _Bool AI);
_Bool Turn(Tabellone* tavolo, Giocatore* giocatore);
_Bool Turn_AI(Tabellone* tavolo, Giocatore* giocatore);

#endif //CLUEDO_BOARDMANAGER_H
