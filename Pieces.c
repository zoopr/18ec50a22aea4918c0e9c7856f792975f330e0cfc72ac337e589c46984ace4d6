//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
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


    mazzo = shuffleDeck_improved(mazzo, mazzo->numCarte); //mescoliamo il mazzo prima di presentarlo all'esterno.
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
    if (!mazzo->numCarte){ //Se per qualsiasi motivo la coda della lista non era terminata correttamente. Può essere artifatto di malloc()
        mazzo->cima = NULL;
    }
    return testa;
}

Mazzo* shuffleDeck_improved(Mazzo* mazzo, int numCarte){ //Fisher-Yates in-place. Matematicamente ottimale, anche se con piccoli problemi
    int i, j, k;                                         // a garantire completa casualità con seed generati da operazioni di modulo.
    Carta *ph1, *ph2;
    Carta *ref1, *ref2;

    for(i=numCarte-1; i>0; i--){
        j = rand()%(i+1);
        if(j!=i){
            ref1 = ref2 = mazzo->cima;
            for (k = 0; k < i - 1; k++) {
                if (k < j - 1) {
                    ref2 = ref2->next;
                }
                ref1 = ref1->next;
            }
            if (j){
                ph1 = ref1->next; //Sui due elementi da scambiare.
                ph2 = ref2->next;
                ref2->next = ph1;
                ref1->next = ph2;
                ref1 = ph1->next; //Salviamo il link per scambiare gli ultimi link sul posto
                ph1->next = ph2->next;
                ph2->next = ref1;
            }
            else{ //Se j è 0, il nostro reference è in realtà sull'elemento stesso e non quello prima.
                ph1 = ref1->next;
                ref1->next = ref2;
                ph2 = ref2->next;
                ref2->next = ph1->next;
                ph1->next = ph2;
                mazzo->cima = ph1; //Aggiorniamo il nuovo primo elemento.
            }

        }
    }

    return mazzo;
}

