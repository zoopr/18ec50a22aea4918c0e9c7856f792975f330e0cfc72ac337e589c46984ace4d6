//
// Created by Mat on 18/01/30.
//
// Control flow della partita e interazioni in-game.
//

#include <stdio.h>
#include <stdlib.h>
#include "BoardManager.h"

Giocatore* playerInit() {
    int num, i;
    Giocatore* listaGiocatori;

    do{
        printf("inserire il numero di giocatori (da %d a %d)\n", GIOCATORI_MIN, GIOCATORI_MAX);
        scanf("%d", &num);
    }while(num>GIOCATORI_MAX || num <GIOCATORI_MIN );


    listaGiocatori = (Giocatore*)calloc(num, sizeof(Giocatore));
    for(i=0; i<num; i++){
        printf("Inserire il nome del giocatore %d (max %d caratteri)\n", i+1, STANDARD_STRLEN - 1);
        scanf("%23s", listaGiocatori[i].nome);
    }

    return listaGiocatori;
}


void MainGame(Tabellone* tavolo){
    //TODO
}


Tabellone* FreshStart(){
    return NULL; //TODO
}


Tabellone* LoadBoard(char* filename){
    return NULL; //TODO
}