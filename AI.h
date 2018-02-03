//
// Created by Mat on 18/01/31.
//
// Logica decisionale AI.
// Decisioni sulla ipotesi basate sugli eventi dei turni precedenti e sulle carte a conoscenza dell'AI lungo la partita.
// Decisioni su che carte mostrare a second di quali sono state mostrate.
// R/W di queste informazioni in maniera azzerabile in sicurezza a ogni partita.
// Le funzioni sono commentate lungo il sorgente.
//

#ifndef CLUEDO_AI_H
#define CLUEDO_AI_H

#include "Pieces.h"

#define INTEREST_FACTOR 1.5

void printTableStatus (Tabellone* tavolo, _Bool AI);
void initInterest(Tabellone* tavolo, Giocatore* giocatore, float loadArea[CARD_TYPES][STANZE_N]);
void saveInterest(Giocatore* giocatore, float loadArea[CARD_TYPES][STANZE_N]);
void readInterest(Tabellone* tavolo, Giocatore* giocatore, float loadArea[CARD_TYPES][STANZE_N]);
void generateCoordinates(Carta* carta, int coords[2]);
int movementStrategy(const float interesseStanze[STANZE_N], const _Bool reachable[STANZE_N]);
int suspectStrategy(const float interesse_AoS[ARMI_N]);
int showingStrategy(Giocatore* giocatore, const int coords[], int len);
#endif //CLUEDO_AI_H
