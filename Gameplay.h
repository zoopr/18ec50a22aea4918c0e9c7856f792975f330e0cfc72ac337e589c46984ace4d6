//
// Created by Mat on 18/01/30.
//
// Controllo degli step logici all'interno del turno.
// Gestione delle task principali di read/write su disco e delle varie routine lungo i turni.
// L'acquisizione dati dall'utente è gestita tramite strtol, che permette di sanitizzare l'input non-numerico settandolo a 0.
// I menu sono presentati all'utente a partire da 1, sia per convenienza che per la gestione dell'eccezione appena accennata.
// Le funzioni sono commentate lungo il sorgente.
//
// NOTA SUL TACCUINO
// Il taccuino non ha alcun dato di identificazione se non il nome dele giocatore.
// Cominciare una partita con lo stesso nome giocatore sovrascrive il contenuto del taccuino.
// Continuare una partita precedente dopo tale evento dichiara informazioni incorrette
// sulle carte scoperte fino a quel momento.
//

#ifndef CLUEDO_GAMEPLAY_H
#define CLUEDO_GAMEPLAY_H

#include "Standards.h"
#include "Pieces.h"

#define D_SIDES 6 //se si vuole giocare con dadi diversi.
#define D_N 2 //Macro per il numero di dadi. Vivamente sconsigliato modificarla.


void rollDice(int dice[D_N]);
void validPaths(const int layout[STANZE_N], int val, _Bool out[STANZE_N]);
int checkSolution(const char* stanza,const char* arma,const char* sospetto, Tabellone* tavolo, _Bool AI, float interestFile[CARD_TYPES][STANZE_N], Taccuino* tac);
int checkCard(const char* stanza,const char* arma,const char* sospetto, Carta* card);
void saveState(char* filename, Tabellone* board);
Taccuino leggiTaccuino(char* filename);
void aggiornaTaccuino(Taccuino* tac, Carta carta);
void scriviTaccuino(char* filename, Taccuino* tacFile);
void logger(char* message);
void statInit(Tabellone* tavolo);
void statSave(Tabellone* tavolo);
void statTrack(Tabellone* tavolo);
void statShow();
int checkCard_Archive(char* (*func)(int, char*), Carta* card, int len);

#endif //CLUEDO_GAMEPLAY_H
