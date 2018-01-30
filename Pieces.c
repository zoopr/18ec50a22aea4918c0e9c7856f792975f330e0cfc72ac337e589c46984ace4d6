//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <string.h>
#include "Pieces.h"

Mazzo* buildDeck(tipoCarta tipo, int numCarte, const char values[][STANDARD_STRLEN]){
    int i;

    Carta* lista = (Carta*)calloc(numCarte, sizeof(Carta)); //Ora sono consecutive, ma la loro posizione in memoria
                                                            // rimane statica nel dealing e shuffling.
    if(!lista){
        exit(-1);
    }
    for (i=0; i<numCarte; i++){
        lista[i].tipo = tipo;
        strcpy(lista[i].desc, values[i]);
        //A questo punto abbiamo un vettore di carte dello stesso tipo.
        //Costruiamo una lista (per ora) consecutiva
        if(i>0){ //allacciamo tutte tranne l'ultima.
            lista[i-1].next = &lista[i]; //l'ultima è già inizializzata a 0 via calloc.
        }
        if (i<numCarte-1){ //idem per la prima.
            lista[i+1].prev = &lista[i];
        }
    }

    Mazzo* mazzo = (Mazzo*)malloc(sizeof(Mazzo));
    if(!mazzo){
        exit(-1);
    }
    mazzo->numCarte = numCarte;
    mazzo->cima = lista;
    mazzo->coda = &lista[numCarte-1];
    return shuffleDeck(mazzo); //mescoliamo il mazzo prima di presentarlo all'esterno.
}

Mazzo* mergeDecks(Mazzo* m1, Mazzo* m2){
    //possiamo semplicemente aggiungerli in coda l'uno all'altro. abbiamo una funzione di shuffling se vogliamo.
    //l'importante è AGGIORNARE IL NUMERO DI CARTE

    m1->coda->next = m2->cima;
    m1->numCarte = m1->numCarte + m2->numCarte;
    free(m2); //abbiamo unito i due mazzi ed estratto tutte le informazioni dal secondo contenitore di lista.
    return m1;
}

Mazzo* shuffleDeck(Mazzo* mazzo){

    //todo
    return mazzo;

}


