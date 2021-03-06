//
// Created by Mat on 18/01/31.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AI.h"
#include "Gameplay.h"

void printTableStatus (Tabellone* tavolo, _Bool AI){ //Usato a inizio turno,  anche nella versione umana.
    Carta* carta;                                    //Rende il taccuino responsabile solo per le carte scoperte dagli avversari.
    int i, j;
    char buffer[SBUF];

    printf("Le carte sul tavolo sono %d.\n", tavolo->carteScoperte.numCarte);
    carta = tavolo->carteScoperte.cima;
    for (j=0; j<tavolo->carteScoperte.numCarte; j++){
        printf("\t%-8s : %s\n", tipi(carta->tipo, buffer), carta->desc);
        if (carta->next)
            carta = carta->next;
    }
    if(AI){
        for(i=0; i<tavolo->numGiocatori; i++) {
            printf("Le carte nella mano di %s sono %d.\n", tavolo->giocatori[i].nome, tavolo->giocatori[i].mano.numCarte);
            carta = tavolo->giocatori[i].mano.cima;
            for (j = 0; j < tavolo->giocatori[i].mano.numCarte; j++) {
                printf("\t%-8s : %s\n", tipi(carta->tipo, buffer), carta->desc);
                if (carta->next)
                    carta = carta->next;
            }
        }
        printf("Le carte segrete sono %d\n", tavolo->soluzione.numCarte);
        carta = tavolo->soluzione.cima;
        for (j=0; j<tavolo->soluzione.numCarte; j++){
            printf("\t%-8s : %s\n", tipi(carta->tipo, buffer), carta->desc);
            if (carta->next)
                carta = carta->next;
        }
        printf("\n");
    }
    else{
        printf("Le carte in mano al giocatore sono %d\n", tavolo->giocatori[tavolo->turnoCorrente].mano.numCarte);
        carta = tavolo->giocatori[tavolo->turnoCorrente].mano.cima;
        for (j = 0; j < tavolo->giocatori[tavolo->turnoCorrente].mano.numCarte; j++) {
            printf("\t%-8s : %s\n", tipi(carta->tipo, buffer), carta->desc);
            if (carta->next)
                carta = carta->next;
        }
    }
}

void readInterest(Tabellone* tavolo, Giocatore* giocatore, float loadArea[CARD_TYPES][STANZE_N]){ //Legge (o inizializza se mancante/danneggiato a metà partita) la matrice d'interesse del giocatore.
    char buf[SBUF];
    strncpy(buf, giocatore->nome, STANDARD_STRLEN);
    strcat(buf, ".ai");
    FILE* interest = fopen(buf, "r+");
    if(interest && !feof(interest)){ // Ci assicura che al primo turno sia generato correttamente.
        fread(loadArea, CARD_TYPES, STANZE_N*sizeof(float), interest);
        fclose(interest);
    }else { //Inizializza un diagramma interno e crea il file.
        initInterest(tavolo, giocatore, loadArea);
        saveInterest(giocatore, loadArea);
    }
}

void saveInterest(Giocatore* giocatore, float loadArea[CARD_TYPES][STANZE_N]){ //salva la nostra matrice di nodi d'interesse su un file specifico al giocatore.
    char buf[SBUF];
    strncpy(buf, giocatore->nome, STANDARD_STRLEN);
    strcat(buf, ".ai");
    FILE* interest = fopen(buf, "w");
    if(!interest){
        exit(-2);
    }
    fwrite(loadArea, CARD_TYPES, STANZE_N*sizeof(float), interest);
    fclose(interest);
}

void initInterest(Tabellone* tavolo, Giocatore* giocatore, float loadArea[CARD_TYPES][STANZE_N]){ //Inizializza le liste di interesse
    int i[2], j;
    Carta* carta;
    for (i[0] = 0; i[0] < CARD_TYPES; i[0]++) {
        for (i[1]=0; i[1]<STANZE_N; i[1]++){
            if(i[0]!=STANZA && i[1]>=ARMI_N){ //azzerare le opzioni ARMI e SOSPETTI che non possiamo selezionare. L'algoritmo di selezione le restringe comunque.
                loadArea[i[0]][i[1]] = 0.0f;
            }else{
                loadArea[i[0]][i[1]] = 1.0f;
            }
        }
    }
    //Eliminiamo le carte sul tavolo.
    carta = tavolo->carteScoperte.cima;
    for(j=0; j<tavolo->carteScoperte.numCarte; j++){
        generateCoordinates(tavolo, carta, i);
        loadArea[i[0]][i[1]] = 0.0f;
        carta = carta->next;
    }
    //Eliminiamo le carte in mano al giocatore.
    carta = giocatore->mano.cima;
    for(j=0; j<giocatore->mano.numCarte; j++){
        generateCoordinates(tavolo, carta, i);
        loadArea[i[0]][i[1]] = 0.0f;
        carta = carta->next;
    }
}

void generateCoordinates(Tabellone* tavolo, Carta* carta, int coords[2]){ // Genera le coordinate spaziali nella matrice dell'interesse per una carta.
    coords[0] = carta->tipo;
    switch(coords[0]){
        case STANZA:
            coords[1] = checkCard_Archive(tavolo->stringhe, carta, STANZE_N);
            break;
        case ARMA:
            coords[1] = checkCard_Archive(tavolo->stringhe, carta, ARMI_N);
            break;
        default:
            coords[1] = checkCard_Archive(tavolo->stringhe, carta, SOSPETTI_N);
            break;
    }
}

int movementStrategy(float interesseStanze[STANZE_N], _Bool reachable[STANZE_N], int layout[STANZE_N][STANZE_N]){ //Decisione direttamente proporzionale al livello d'interesse su quella casella.
    float interesseCorretto[STANZE_N];
    _Bool newMask[STANZE_N];
    int i, j, lastIndex;
    float sum, incremental, seed;
    for(i=0; i<STANZE_N; i++){
        interesseCorretto[i] = interesseStanze[i]*(float)reachable[i]; //azzera le stanze a cui non possiamo accedere questo turno.
    }

    sum = 0.0f;
    for(i=0; i<STANZE_N; i++) {
        sum += interesseCorretto[i];
    }

    seed = (float)rand()/(float)RAND_MAX; //Generiamo un float da 0 a 1.
    incremental = 0.0f;

    if(sum > 0.0f) {
        for (i = 0; i < STANZE_N; i++) {
            incremental += (interesseCorretto[i] / sum);
            if (seed < incremental) { // Quando cade sulla fetta di un certo indice, ritorniamo quell'indice.
                return i;
            }
        }
    } else { // Se nessuna delle stanze di cui abbia effetivo interesse è raggiungibile, l'AI si muove secondo un pathing più ottimale possibile verso un punto di interesse.
        for(i=0, j=0; i<STANZE_N; i++){ // Incluso stare ferma se possibile.
            if(interesseStanze[i]>0.0f){ // Controlliamo se aveva una specifica destinazione in mente o se voleva investigare più di una stanza.
                j++;
                lastIndex = i;
            }
            interesseCorretto[i] = (float)reachable[i];
            newMask[i] = 1;
        }
        // Se aveva una singola destinazione in mente, massimizziamo il movimento per quel punto.
        // Altrimenti massimizziamo la distanza verso almeno uno dei punti di interesse deciso a caso (sempre con probabilità direttamente proporzionale).
        if(j > 1) { // j è almeno 1 e lastIndex è sempre inzializzato, a meno di errori nel R/W della matrice di interesse che sono catturati nello scopo superiore con uscita -3.
            lastIndex = movementStrategy(interesseCorretto,
                                         newMask, layout); //In questa iterazione la maschera sono i float e il peso sono i bool, tutti uguali e positivi.
        }                                        // Dunque ha sempre soluzione casuale tra le stanze raggiungibili e non raggiunge mai questo branch ricorsivamente
        return BestPath(reachable, lastIndex, layout);

    }
}

int suspectStrategy(float interesse_AoS[ARMI_N]){ //Decisione per sospetti e armi. Direttamente proporzionale al peso in "interesse".
    int i;
    float sum, incremental, seed;
    sum = 0.0f;
    for(i=0; i<ARMI_N; i++) {
        sum += interesse_AoS[i];
    }

    seed = (float)rand()/(float)RAND_MAX; //Generiamo un float da 0 a 1.
    incremental = 0.0f;
    for(i=0; i<ARMI_N; i++){
        incremental += (interesse_AoS[i]/sum);
        if (seed < incremental){ // Quando cade sulla fetta di un certo indice, ritorniamo quell'indice.
            return i;
        }
    }
    /*
     * L'AI NON PUO' avere tutti gli interessi per armi o sospetti a 0 allo stesso tempo.
     * Se abbiamo sbordato qua, probabilmente per un errore di R/W dei nodi di interesse,
     * si comunica l'errore.
     */
    fprintf(stderr, "SCELTA FUORI INDICE\n");
    exit(-4);
}

int showingStrategy(Giocatore* giocatore, const int coords[], int len){ //Decide quali carte mostrare e aggiorna quelle mostrate.
    _Bool shown[6] = {0, 0, 0, 0, 0, 0};
    char buf[SBUF];
    int i;
    FILE* shownMem;

    strncpy(buf, giocatore->nome, STANDARD_STRLEN);
    strcat(buf, "_hold.ait");
    shownMem = fopen(buf, "w+");
    if (!shownMem) //Problema di generazione file.
        exit(-2);
    if(len != 0){
        fread(shown, 6, sizeof(_Bool), shownMem);// Carichiamo i dati nel vettore.

        for (i = 0; i <
                    len; i++) { //Scrolling nella nostra mano per vedere se abbiamo già presentato una carta agli indici contenuti in coords[]
            if (shown[coords[i]]) {
                return i; //Ovviamente non richiede di aggiornare la nostra memoria.
            }
        }
        //Se siamo qua, nessuna carta che abbiamo già mostrato può essere mostrata.
        //Se nessuna delle carte da mostrare era stata mostrata prima, prendiamo la prima e registriamo che sia stata mostrata.
        shown[coords[0]] = 1;
    }
    fwrite(shown, 6, sizeof(_Bool), shownMem);
    fclose(shownMem);
    return 0;
}

int BestPath(_Bool reachable[STANZE_N], int end, int layout[STANZE_N][STANZE_N]){ // Ritorna il nodo più vicino alla destinazione tra quelli raggiungibili.
    int i, minIndex, minDist = 99; // MinDist ci permette di non dover inizializzare minIndex con un'iterazione inutile lungo reachable.

    /* Tutti i nodi sono connessi tra loro con distanza ottimale. Ciò significa che se il collegamento in un passo non è disponibile
     * e la destinazione non è il nodo più vicino alla partenza, esiste almeno un altro percorso di distanza ottimale in 2 o più passi.
     * Non c'è bisogno di nessun pathing: si tratta solo di valutare quale offra i dadi più efficaci a raggiungere la destinazione al prossimo turno,
     * cioè la distanza minima dal nodo prescelto.
     */
    for(i=0; i<STANZE_N; i++){
        if(reachable[i]){
            if (layout[i][end] < minDist){
                minIndex = i;
                minDist = layout[minIndex][end];
            }
        }
    }
    return minIndex; // MinDist, e per estensione minIndex, è sempre inizializzato dato che reachable contiene sempre la partenza
                     // e ogni nodo è connesso a ognuno degli altri con distanza tra 0 e 16 (MinDist è settato a 99).
}