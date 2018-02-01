//
// Created by Mat on 18/01/30.
//
// Controllo degli step logici all'interno del turno.
// Gestione delle task principali di read/write su disco e delle varie routine lungo i turni.
// Le funzioni sono commentate lungo il sorgente.
//
// NOTA SUL TACCUINO
// Il taccuino non ha alcun dato di identificazione se non il nome dele giocatore.
// Cominciare una partita con lo stesso nome giocatore sovrascrive il contenuto del taccuino.
// Continuare una partita precedente dopo tale evento dichiara informazioni incorrette
// sulla propria mano e il tavolo in console all'inizio di ogni turno del giocatore.
//

#ifndef CLUEDO_GAMEPLAY_H
#define CLUEDO_GAMEPLAY_H

#include "Standards.h"
#include "Pieces.h"

#define D_SIDES 6 //giusto in caso si voglia giocare con dadi diversi.
#define D_N 2 //se si vuole aumentare il numero. Non diminuire. Varie grandezze sono usate come vettori lunghi almeno 2.

void leggiTaccuino(char* filename);
void rollDice(int dice[D_N]);
void validPaths(const int layout[STANZE_N], int val, _Bool out[STANZE_N]);
int checkSolution(const char*stanza,const char* arma,const char* sospetto, Tabellone* tavolo, _Bool AI, float interestFile[CARD_TYPES][STANZE_N]);
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
