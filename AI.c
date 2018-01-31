//
// Created by Mat on 18/01/31.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AI.h"
#include "Gameplay.h"

void printTableStatus (Tabellone* tavolo){ // preso parzialmente dalla inizializzazione del interestcuino. Aggiunte le funzionalità necessarie nel turno AI.
    Carta* carta;
    int i, j;
    char buffer[SBUF];

    printf("Le carte sul tavolo sono %d\n", tavolo->carteScoperte.numCarte);
    carta = tavolo->carteScoperte.cima;
    for (j=0; j<tavolo->carteScoperte.numCarte; j++){
        printf("%s : %s\n", tipi(carta->tipo, buffer), carta->desc);
        if (carta->next)
            carta = carta->next;
    }
    for(i=0; i<tavolo->numGiocatori; i++) {
        printf("Le carte nella mano di %s sono %d\n", tavolo->giocatori[i].nome, tavolo->giocatori[i].mano.numCarte);
        carta = tavolo->giocatori[i].mano.cima;
        for (j = 0; j < tavolo->giocatori[i].mano.numCarte; j++) {
            printf("%s : %s\n", tipi(carta->tipo, buffer), carta->desc);
            if (carta->next)
                carta = carta->next;
        }
    }
    printf("Le carte segrete sono %d\n", tavolo->soluzione.numCarte);
    carta = tavolo->soluzione.cima;
    for (j=0; j<tavolo->soluzione.numCarte; j++){
        printf("%s : %s\n", tipi(carta->tipo, buffer), carta->desc);
        if (carta->next)
            carta = carta->next;
    }
}

void readInterest(Tabellone* tavolo, float loadArea[3][STANZE_N]){
    char buf[SBUF];
    int i;
    Giocatore* giocatore = &tavolo->giocatori[tavolo->turnoCorrente];
    strcpy(buf, giocatore->nome);
    strcat(buf, ".ai");
    FILE* interest = fopen(buf, "r");

    if(interest && !feof(interest)){ // Ci assicura che nella prima partita del sistema sia gestito correttamente.
        fread(loadArea, 3, STANZE_N*sizeof(float), interest);
        fclose(interest);
    }else { //Inizializza un diagramma interno e crea il file.
        initInterest(tavolo, loadArea);
        saveInterest(tavolo, loadArea);
    }
}

void saveInterest(Tabellone* tavolo, float loadArea[3][STANZE_N]){ //salva la nostra matrice di nodi d'interesse su un file specifico al giocatore.
    char buf[SBUF];
    Giocatore* giocatore = &tavolo->giocatori[tavolo->turnoCorrente];
    strcpy(buf, giocatore->nome);
    strcat(buf, ".ai");
    FILE* interest = fopen(buf, "w");
    if(!interest){
        exit(-2);
    }
    fwrite(loadArea, 3, STANZE_N*sizeof(float), interest);
    fclose(interest);
}

void saveInterest_init(char* filename, float loadArea[3][STANZE_N]){ //Azzerare su reinizializzazione. Stesso problema dei taccuini.
    char buf[SBUF];
    strcpy(buf, filename);
    strcat(buf, ".ai");
    FILE* interest = fopen(buf, "w");
    if(!interest){
        exit(-2);
    }
    fwrite(loadArea, 3, STANZE_N*sizeof(float), interest);
    fclose(interest);
}

void initInterest(Tabellone* tavolo, float loadArea[3][STANZE_N]){ //Inizializza le liste di interesse
    int i[2];
    Carta* carta;

    for (i[0] = 0; i[0] < 3; i[0]++) {
        for (i[1]=0; i[1]<STANZE_N; i[1]++){
            if(i[0]>=1 && i[1]>=ARMI_N){ //useremo questo spazio per gestire quali carte abbiamo mostrato. todo
                loadArea[i[0]][i[1]] = 0.0f;
            }else{
                loadArea[i[0]][i[1]] = 1.0f;
            }
        }
    }
    //Eliminiamo immediatamente le carte in mano e sul tavolo
    carta = tavolo->carteScoperte.cima;
    while(carta){
        generateCoordinates(carta, i);
        loadArea[i[0]][i[1]] = 0.0f;
        carta = carta->next;
    }
    carta = tavolo->giocatori[tavolo->turnoCorrente].mano.cima;
    while(carta){
        generateCoordinates(carta, i);
        loadArea[i[0]][i[1]] = 0.0f;
        carta = carta->next;
    }
}

void initInterest_Global(Tabellone* tavolo, Giocatore* giocatore, float loadArea[3][STANZE_N]){
    int i[2];
    Carta* carta;

    for (i[0] = 0; i[0] < 3; i[0]++) {
        for (i[1]=0; i[1]<STANZE_N; i[1]++){
            if(i[0]>=1 && i[1]>=ARMI_N){ //useremo questo spazio per gestire quali carte abbiamo mostrato. todo?
                loadArea[i[0]][i[1]] = 0.0f;
            }else{
                loadArea[i[0]][i[1]] = 1.0f;
            }
        }
    }
    //Eliminiamo immediatamente le carte in mano e sul tavolo
    carta = tavolo->carteScoperte.cima;
    while(carta){
        generateCoordinates(carta, i);
        loadArea[i[0]][i[1]] = 0.0f;
        carta = carta->next;
    }
    carta = giocatore->mano.cima;
    while(carta){
        generateCoordinates(carta, i);
        loadArea[i[0]][i[1]] = 0.0f;
        carta = carta->next;
    }
}

void generateCoordinates(Carta* carta, int coords[2]){ // Genera le coordinate spaziali nella matrice dell'interesse per una carta.
    coords[0] = carta->tipo;
    switch(coords[0]){
        case STANZA:
            coords[1] = checkCard_Archive(stanze, carta, STANZE_N);
            break;
        case ARMA:
            coords[1] = checkCard_Archive(armi, carta, ARMI_N);
            break;
        default:
            coords[1] = checkCard_Archive(sospetti, carta, SOSPETTI_N);
            break;
    }
}

int movementStrategy(const float interesseStanze[STANZE_N],const _Bool reachable[STANZE_N]){ //Decisione direttamente proporzionale al livello d'interesse su quella casella.
    float interesseCorretto[STANZE_N];
    int i;
    float sum, incremental, seed;
    for(i=0; i<STANZE_N; i++){
        interesseCorretto[i] = interesseStanze[i]*reachable[i]; //azzera le stanze a cui non possiamo accedere questo turno.
    }
    sum = 0.0f;
    for(i=0; i<STANZE_N; i++) {
        sum += interesseCorretto[i];
    }

    seed = (float)rand()/(float)RAND_MAX; //Generiamo un float da 0 a 1.
    incremental = 0.0f;
    for(i=0; i<STANZE_N; i++){
        incremental += interesseCorretto[i]/sum;
        if (seed < incremental){ // Quando cade sulla fetta di un certo indice, ritorniamo quell'indice.
            return i;
        }
    }

}

int suspectStrategy(const float interesse_AoS[ARMI_N]){ //Decisione per sospetti e armi.
    int i;
    float sum, incremental, seed;

    sum = 0.0f;
    for(i=0; i<ARMI_N; i++) {
        sum += interesse_AoS[i];
    }

    seed = (float)rand()/(float)RAND_MAX; //Generiamo un float da 0 a 1.
    incremental = 0.0f;
    for(i=0; i<ARMI_N; i++){
        incremental += interesse_AoS[i]/sum;
        if (seed < incremental){ // Quando cade sulla fetta di un certo indice, ritorniamo quell'indice.
            return i;
        }
    }
}
