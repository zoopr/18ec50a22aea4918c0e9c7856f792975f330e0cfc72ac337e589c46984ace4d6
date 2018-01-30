//
// Created by Mat on 18/01/30.
//
// Control flow della partita e interazioni in-game.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BoardManager.h"

Giocatore* playerInit(int* num) {
    int i;
    Giocatore* listaGiocatori;


    do{
        printf("inserire il numero di giocatori (da %d a %d)\n", GIOCATORI_MIN, GIOCATORI_MAX);
        scanf("%d", num);
    }while(*num>GIOCATORI_MAX || *num <GIOCATORI_MIN );


    listaGiocatori = (Giocatore*)calloc(*num, sizeof(Giocatore));
    if (!listaGiocatori){
        exit(-1);
    }
    for(i=0; i<*num; i++){
        printf("Inserire il nome del giocatore %d (max %d caratteri)\n", i+1, STANDARD_STRLEN - 1);
        scanf("%23s", listaGiocatori[i].nome);
        listaGiocatori[i].ipotesiEsatta = 0;
        listaGiocatori[i].mano.numCarte = 0;
        listaGiocatori[i].mano.cima = NULL;
        listaGiocatori[i].stanza = rand()%STANZE_N;
    }

    return listaGiocatori;
}





Tabellone* FreshStart(){ //Inizializza il tavolo
    int numGiocatori;
    Mazzo *mainDeck, *second, *third;

    Tabellone* tavolo = (Tabellone*)malloc(sizeof(Tabellone));
    if(!tavolo){
        exit(-1);
    }
    memcpy(tavolo->layout, moveset, STANZE_N*STANZE_N*sizeof(int)); //copia dai dati di specifica dell'array layout del tavolo
    tavolo->giocatori = playerInit(&numGiocatori); //salviamo il numero all'interno di numGiocatori per le funzioni successive.
    tavolo->numeroTurni = 0;
    tavolo->numGiocatori = numGiocatori;
    tavolo->turnoCorrente = rand()%numGiocatori;


    //Funzioni di shuffling e dealing del mazzo.
    mainDeck = buildDeck(STANZA, STANZE_N, stanze);
    second = buildDeck(ARMA, ARMI_N, armi);
    third = buildDeck(SOSPETTO, SOSPETTI_N, sospetti);

    printf("Mazzi inizializzati.\n");
    //peschiamo dalla cima del mazzo.
    tavolo->soluzione.cima = mainDeck->cima;
    mainDeck->cima = mainDeck->cima->next;
    mainDeck->cima->prev = NULL;
    tavolo->soluzione.cima->next = second->cima;
    tavolo->soluzione.cima->next->prev = tavolo->soluzione.cima; //
    second->cima = second->cima->next; //aggiorniamo la seconda lista
    second->cima->prev = NULL;
    tavolo->soluzione.cima->next->next = tavolo->soluzione.coda = third->cima;
    third->cima = third->cima->next;
    third->cima->prev = NULL;
    tavolo->soluzione.cima->next->next->prev = tavolo->soluzione.cima->next;
    tavolo->soluzione.cima->next->next->next = NULL;

    tavolo->soluzione.numCarte = 3;
    printf("Omicidio eseguito.\n");


    mainDeck = mergeDecks(mainDeck, mergeDecks(second, third)); //uniamo i tre mazzi in fila
    mainDeck = shuffleDeck(mainDeck); //mescoliamo un'altra volta.
    printf("Uniti i mazzi.\n");
    //todo distribuire le carte restanti

    return tavolo;

}


Tabellone* LoadBoard(char* filename){
    return NULL; //TODO
}

void MainGame(Tabellone* tavolo){
    //TODO
}