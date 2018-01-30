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
    char buffer[STANDARD_STRLEN + 4];

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
    tavolo->soluzione.cima->next->next = third->cima;
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
        tavolo->giocatori[i].mano.numCarte = cartePerGiocatore;
    }
    if(!mainDeck->numCarte)
        printf("Carte distribuite correttamente. Tavolo pronto.\n");

    for (i=0; i<numGiocatori; i++){
        strcpy(buffer, tavolo->giocatori[i].nome);
        strcat(buffer, ".tac");
        fopen(buffer, "w"); //la tag w ci assicura che se esiste viene azzerato.
        fclose(buffer); // ci permette di usare "a+" correttamente lungo i turni dopo.
    }

    return tavolo;

}


Tabellone* LoadBoard(char* filename){
    int i, j;
    Giocatore ultimoG, *codaG;
    Carta *ultimaC, *codaC;

    FILE* save = fopen(filename, "r+");
    if (!save){
        printf("Errore nel caricamento del file\n"
        "Verificare il nome e la posizione del file di salvataggio.\n");
        return NULL;
    }
    else {
        Tabellone *table = (Tabellone *) malloc(sizeof(Tabellone)); //seguendo il formato byte a byte delle specifiche.
        if (!table)
            exit(-1);
        fread(table, 3, sizeof(int), save);
        table->giocatori = (Giocatore *) calloc(table->numGiocatori, sizeof(Giocatore));
        for (i = 0; i < table->numGiocatori; i++) {
            ultimoG = table->giocatori[i];
            fread(&ultimoG, 1, STANDARD_STRLEN * sizeof(char) + 2 * sizeof(int), save);
            fread(&ultimoG.mano.numCarte, 1, sizeof(int), save);
            for (j = 0; j < ultimoG.mano.numCarte; j++) {
                ultimaC = (Carta *) malloc(sizeof(Carta));
                if (!ultimaC)
                    exit(-1);
                fread(ultimaC, 1, sizeof(tipoCarta) + STANDARD_STRLEN * sizeof(char), save);
                if (!ultimoG.mano.cima) {
                    ultimoG.mano.cima = ultimaC;
                    ultimaC->next = NULL;
                } else {
                    codaC = ultimoG.mano.cima;
                    while (codaC->next)
                        codaC = codaC->next;
                    codaC->next = ultimaC;
                }

            }

        }
        fread(&table->carteScoperte.numCarte, 1, sizeof(int), save);
        for (i = 0; i < table->carteScoperte.numCarte; i++) {
            ultimaC = (Carta *) malloc(sizeof(Carta));
            if (!ultimaC)
                exit(-1);
            fread(ultimaC, 1, sizeof(tipoCarta) + STANDARD_STRLEN * sizeof(char), save);
            if (!table->carteScoperte.cima) {
                table->carteScoperte.cima = ultimaC;
                ultimaC->next = NULL;
            } else {
                codaC = table->carteScoperte.cima;
                while (codaC->next)
                    codaC = codaC->next;
                codaC->next = ultimaC;
            }

        }
        table->soluzione.numCarte = 3;
        table->soluzione.cima = 0;
        for (i = 0; i < table->soluzione.numCarte; i++) {
            ultimaC = (Carta *) malloc(sizeof(Carta));
            if (!ultimaC)
                exit(-1);
            fread(ultimaC, 1, sizeof(tipoCarta) + STANDARD_STRLEN * sizeof(char), save);
            if (!table->soluzione.cima) {
                table->soluzione.cima = ultimaC;
                ultimaC->next = NULL;
            } else {
                codaC = table->soluzione.cima;
                while (codaC->next)
                    codaC = codaC->next;
                codaC->next = ultimaC;
            }

        }
        fclose(save);
        //non ci preoccupiamo dei taccuini. La tag "a" crea i file se non esistenti.
        return table;
    }


}

void MainGame(Tabellone* tavolo){
    _Bool winner = 0;
    while(!winner){
        tavolo->turnoCorrente = (tavolo->turnoCorrente+1)%tavolo->numGiocatori;
        tavolo->numeroTurni +=1;
        winner = Turn(tavolo, &tavolo->giocatori[tavolo->turnoCorrente]);
    }
    printf("  ___ ___  _ __   __ _ _ __ __ _| |_ _   _| | __ _ ___(_) ___  _ __ (_) |\n"
                   " / __/ _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` |_  / |/ _ \\| '_ \\| | |\n"
                   "| (_| (_) | | | | (_| | | | (_| | |_| |_| | | (_| |/ /| | (_) | | | | |_|\n"
                   " \\___\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_/___|_|\\___/|_| |_|_(_)\n"
                   "                 |___/                                                   \n"
            "\n%s ha vinto!\n", tavolo->giocatori[tavolo->turnoCorrente].nome);
    //todo record stats
    printf("Premere un pulsante per terminare\n");
    getchar();
}

_Bool Turn(Tabellone* tavolo, Giocatore* giocatore){

}