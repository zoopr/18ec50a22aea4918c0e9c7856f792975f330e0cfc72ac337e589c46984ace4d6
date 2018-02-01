//
// Created by Mat on 18/01/30.
//
// Organizza le strutture corrispondenti ai pezzi logici del tabellone.
// Contiene le strutture di base essenziali al gameplay.
// Le funzioni sono commentate lungo il sorgente.
//

#ifndef CLUEDO_PIECES_H
#define CLUEDO_PIECES_H

#include "Standards.h"

typedef struct card{
    tipoCarta tipo;
    char desc[STANDARD_STRLEN];
    struct card* next;
} Carta;

typedef struct deck{ // Usato sia per i mazzi sul tavolo che per le mani dei giocatori.
    int numCarte;
    Carta* cima;
} Mazzo;

typedef struct player{
    char nome[STANDARD_STRLEN];
    int stanza;
    int ipotesiEsatta;
    Mazzo mano;
} Giocatore;

typedef struct board{
    int turnoCorrente;
    int numeroTurni;
    int numGiocatori;
    Giocatore* giocatori; //Allochiamo i giocatori in una lista di lunghezza decisa a runtime.
    Mazzo carteScoperte;
    Mazzo soluzione;
    int layout[STANZE_N][STANZE_N];
    int stats[3][STANZE_N];
} Tabellone;

Mazzo* buildDeck(tipoCarta tipo, int numCarte, char* (*func)(int, char*));
Mazzo* mergeDecks(Mazzo* m1, Mazzo* m2);
Carta* DealCards(Mazzo* mazzo, int numCarte);
Mazzo* shuffleDeck_improved(Mazzo* mazzo, int numCarte);

#endif //CLUEDO_PIECES_H
