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
    int numGiocatori, i, cartePerGiocatore;
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
    tavolo->soluzione.cima->next = second->cima;
    second->cima = second->cima->next;
    tavolo->soluzione.cima->next->next = tavolo->soluzione.coda = third->cima;
    third->cima = third->cima->next;
    tavolo->soluzione.cima->next->next->next = NULL;

    mainDeck->numCarte--;
    second->numCarte--;
    third->numCarte--;

    tavolo->soluzione.numCarte = 3;
    printf("Omicidio eseguito.\n");


    mainDeck = mergeDecks(mainDeck, mergeDecks(second, third)); //uniamo i tre mazzi in fila
    second = third = NULL; //per sicurezza visto che abbiamo liberato i mazzi contenitore.
    mainDeck = shuffleDeck(mainDeck, mainDeck->numCarte); //mescoliamo un'altra volta.
    printf("Uniti i mazzi.\n");

    tavolo->carteScoperte.cima = DealCards(mainDeck, mainDeck->numCarte%numGiocatori);
    tavolo->carteScoperte.numCarte = mainDeck->numCarte%numGiocatori;

    cartePerGiocatore = mainDeck->numCarte / numGiocatori;
    for (i = 0; i<numGiocatori; i++){
        tavolo->giocatori[i].mano.cima = DealCards(mainDeck, cartePerGiocatore);
    }
    if(!mainDeck->numCarte)
        printf("Carte distribuite correttamente. Tavolo pronto.\n");

    return tavolo;

}


Tabellone* LoadBoard(char* filename){
    return NULL; //TODO
}

void MainGame(Tabellone* tavolo){
    //TODO
}