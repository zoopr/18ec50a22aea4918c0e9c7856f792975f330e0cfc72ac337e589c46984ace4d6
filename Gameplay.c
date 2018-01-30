//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <stdio.h>
#include "Gameplay.h"

#define D_SIDES 6 //giusto in caso si voglia giocare con dadi diversi.

void leggiTaccuino(char* filename){
    //todo

}


void rollDice(int dice[2]){
    dice[0] = rand()%D_SIDES+1;
    dice[1] = rand()%D_SIDES+1;
    printf("Hai fatto %d + %d\n", dice[0], dice[1]);
}


void validPaths(int layout[STANZE_N], int val, _Bool out[STANZE_N]){
    int i;
    for (i=0; i<STANZE_N; i++){
        if (layout[i]<= val)
            out[i] = 1;
        else
            out[i] = 0;
    }
}

_Bool checkSolution(const char* stanza,const char* arma,const char* sospetto){
//TODO
}

void saveState(char filename[STANDARD_STRLEN], Tabellone* board){
    FILE* save = fopen(filename, "w"); //qualunque cosa ci sia la sovrascrive.
    if (!save)
        exit(-2);
    int i, j;
    Carta* carta;

    fwrite(&board->turnoCorrente, 1, sizeof(int), save); //scrive turnoCorrente, numeroTurni, numeroGiocatori.
    fwrite(&board->numeroTurni, 1, sizeof(int), save);
    fwrite(&board->numGiocatori, 1, sizeof(int), save);

    for (i=0; i< board->numGiocatori; i++){
        fwrite(&board->giocatori[i].nome, 1, STANDARD_STRLEN*sizeof(char), save); //scrive la prima parte dei dati giocatore
        fwrite(&board->giocatori[i].stanza, 1, sizeof(int), save);
        fwrite(&board->giocatori[i].ipotesiEsatta, 1, sizeof(int), save);
        fwrite(&board->giocatori[i].mano.numCarte, 1, sizeof(int), save); // copia il numero di carte
        carta = board->giocatori[i].mano.cima;
        for (j=0; j<board->giocatori[i].mano.numCarte; j++){
            fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //salva la prima parte della carta.
            if (carta->next) //scorre fino all'ultima.
                carta = carta->next;
        }

    }
    fwrite( &board->carteScoperte.numCarte, 1, sizeof(int), save); //sezione identica al salvataggio consecutivo delle carte giocatore.
    carta = board->carteScoperte.cima;
    for (j=0; j<board->carteScoperte.numCarte; j++){
        fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //salva la prima parte della carta.
        if (carta->next) //scorre fino all'ultima.
            carta = carta->next;
    }
    carta = board->soluzione.cima; //ovviamente l'ultimo blocco di carte non ha un int esplicito di lunghezza.
    for (j=0; j<board->soluzione.numCarte; j++){
        fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //salva la prima parte della carta.
        if (carta->next) //scorre fino all'ultima.
            carta = carta->next;
    }
    fclose(save);
}