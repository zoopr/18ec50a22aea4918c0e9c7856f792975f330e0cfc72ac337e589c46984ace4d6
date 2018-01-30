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
} Carta;

typedef struct deck{ //usato sia per i mazzi sul tavolo che per le mani dei giocatori.
    Carta* cima;
} Mazzo;

typedef struct player{
    char nome[STANDARD_STRLEN];
    int stanza;
    int ipotesiEsatta;
    Mazzo mano;
} Giocatore;

typedef struct board{

};

#endif //CLUEDO_PIECES_H
