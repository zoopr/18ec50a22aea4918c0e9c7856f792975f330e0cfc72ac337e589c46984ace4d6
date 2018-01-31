//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Pieces.h"

Mazzo* buildDeck(tipoCarta tipo, int numCarte, char* (*func)(int, char*) ){
    int i;
    Carta vettore[numCarte];
    Carta *lista, *newElem;
    
    for (i=0; i<numCarte; i++){
        vettore[i].tipo = tipo;
        func(i, vettore[i].desc); //copia il valore nella memoria dele parametro 2. vedi funzioni in standards.h
        //A questo punto abbiamo un vettore di carte dello stesso tipo.

    }

    Mazzo* mazzo = (Mazzo*)malloc(sizeof(Mazzo));
    if(!mazzo){
        exit(-1);
    }
    mazzo->numCarte = numCarte;

    lista = newElem = (Carta*)malloc(sizeof(Carta));
    if (!newElem) {
        exit(-1);
    }

    for (i=0; i<numCarte; i++){
        *newElem = vettore[i];
        if(i<numCarte-1) {
            newElem = newElem->next = (Carta *) malloc(sizeof(Carta));
            if (!newElem) {
                exit(-1);
            }
        }
        else
           newElem->next = NULL;
    }
    mazzo->cima = lista;


    mazzo = shuffleDeck(mazzo, mazzo->numCarte); //mescoliamo il mazzo prima di presentarlo all'esterno.
    return mazzo;
}

Mazzo* mergeDecks(Mazzo* m1, Mazzo* m2){
    //possiamo semplicemente aggiungerli in coda l'uno all'altro.
    //l'importante è AGGIORNARE IL NUMERO DI CARTE.
    Carta* scroll = m1->cima;
    while(scroll->next)
        scroll = scroll->next;

    scroll->next = m2->cima;
    m1->numCarte = m1->numCarte + m2->numCarte;

    free(m2); //abbiamo unito i due mazzi ed estratto tutte le informazioni dal secondo contenitore di lista
    return m1;
}

Mazzo* shuffleDeck(Mazzo* mazzo, int numCarte){
    int i, j, k;
    Carta *copy;
    Carta *lista, *newElem;
    Carta rebuild[18];
    _Bool called[18];

    for (i=0; i<numCarte; i++){
        called[i] =0;
    }

    //costruisci un vettore statico random.
    for (i=0; i<numCarte; i++){
        do {
            j = rand() % numCarte;
        }while(called[j]);
        called[j] = 1;
        for(k=0, copy = mazzo->cima; k<j; k++){
            copy = copy->next;
        }
        rebuild[i].tipo = copy->tipo;
        strcpy(rebuild[i].desc, copy->desc);
    }

    //costruisce una lista random dal vettore.
    lista = newElem = (Carta*)malloc(sizeof(Carta));

    for (i=0; i<numCarte; i++){
        *newElem = rebuild[i];
        if(i<numCarte-1)
            newElem = newElem->next = (Carta*)malloc(sizeof(Carta));
        else
            newElem->next = NULL;
    }
    //libera la lista vecchia.
    copy = mazzo->cima;
    while(copy){
        mazzo->cima = copy->next;
        free(copy);
        copy = mazzo->cima;
    }

    mazzo->cima = lista;



    return mazzo;

}

Carta* DealCards(Mazzo* mazzo, int numCarte){ //ritorna numCarte carte, aggiorna il mazzo per cominciare da quel punto.
    Carta *segnaposto, *testa;
    segnaposto = testa =  mazzo->cima;
    int i;
    if(numCarte>0){
        for(i=0; i<numCarte-1; i++){
            segnaposto = segnaposto->next;
        }

        mazzo->cima = segnaposto->next;
        segnaposto->next = NULL;
    }
    mazzo->numCarte -= numCarte;
    return testa;
}

