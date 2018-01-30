//
// Created by Mat on 18/01/30.
//
// Organizza le strutture corrispondenti ai pezzi logici del tabellone.
//

#ifndef CLUEDO_PIECES_H
#define CLUEDO_PIECES_H

#include "Standards.h"

typedef struct card{
    tipoCarta tipo;
    char desc[STANDARD_STRLEN];
    struct card* next;
    struct card* prev;
} Carta;

typedef struct deck{ //usato sia per i mazzi sul tavolo che per le mani dei giocatori.
    int numCarte;
    Carta* cima;
    Carta* coda;
} Mazzo;

typedef struct player{
    char nome[STANDARD_STRLEN];
    int stanza;
    int ipotesiEsatta;
    Mazzo mano;
} Giocatore;

typedef struct board{
    int numGiocatori;
    int numeroTurni;
    int turnoCorrente;
    Giocatore* giocatori; //Allochiamo i giocatori in una lista di lunghezza decisa a runtime.
    Mazzo carteScoperte;
    Mazzo soluzione;
    int layout[STANZE_N][STANZE_N];
} Tabellone;

Mazzo* buildDeck(tipoCarta tipo, int numCarte, const char values[][STANDARD_STRLEN]);
Mazzo* mergeDecks(Mazzo* m1, Mazzo* m2);
Mazzo* shuffleDeck(Mazzo* mazzo);

#endif //CLUEDO_PIECES_H
