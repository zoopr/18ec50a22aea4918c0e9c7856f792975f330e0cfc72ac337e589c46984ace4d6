//
// Created by Mat on 18/01/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BoardManager.h"
#include "Gameplay.h"
#include "AI.h"

Giocatore* playerInit(int* num, _Bool AI) { // Alloca i giocatori e assegna i valori iniziali come la posizione.
    int i;                                  // Nella verisone umana chiede anche un nome per i giocatori.
    Giocatore* listaGiocatori;
    char buf[SBUF], *pos;

    do{
        printf("inserire il numero di giocatori (da %d a %d)\n", GIOCATORI_MIN, GIOCATORI_MAX);
        inputWrapper(buf, &pos);
        *num = strtol(buf, NULL, 10);
    }while(*num>GIOCATORI_MAX || *num <GIOCATORI_MIN );


    listaGiocatori = (Giocatore*)calloc((size_t)*num, sizeof(Giocatore));
    if (!listaGiocatori){
        exit(-1);
    }
    if(!AI){
        for(i=0; i<*num; i++){

            printf("Inserire il nome del giocatore %d (max %d caratteri)\n", i+1, STANDARD_STRLEN - 1);
            inputWrapper(buf, &pos);
            if(!strlen(buf)){
                fprintf(stderr, "Errore: campo nome vuoto.\n");
                i--;
                continue;
            }


            strncpy(listaGiocatori[i].nome, buf, STANDARD_STRLEN);
            strcat(buf, ".tac");
            if(wipeTac(buf)){
                i--;
                continue;
            }
            listaGiocatori[i].ipotesiEsatta = 0;
            listaGiocatori[i].mano.numCarte = 0;
            listaGiocatori[i].mano.cima = NULL;
            listaGiocatori[i].stanza = rand()%STANZE_N;
        }
    }else{
        for(i=0; i<*num; i++){
            strcpy(listaGiocatori[i].nome, "Player ");                          // Crea un nome unico per ogni giocatore AI da un template comune e la loro posizione..
            dtoc(i+1, &listaGiocatori[i].nome[strlen(listaGiocatori[i].nome)]); // Compatibile per essere usata su nomi standard AI minori di 21 caratteri.
            listaGiocatori[i].ipotesiEsatta = 0;
            listaGiocatori[i].mano.numCarte = 0;
            listaGiocatori[i].mano.cima = NULL;
            listaGiocatori[i].stanza = rand()%STANZE_N;
        }
    }


    return listaGiocatori;
}

Tabellone* FreshStart(_Bool AI){ //Inizializza il tavolo. Crea i giocatori e distribuisce le carte.
    int numGiocatori, cartePerGiocatore, i, j;
    Mazzo *mainDeck, *second, *third;
    char buffer[STANDARD_STRLEN + 4];

    Tabellone* tavolo = (Tabellone*)malloc(sizeof(Tabellone));
    if(!tavolo){
        exit(-1);
    }
    copiaMoveset(tavolo->layout); //copia il layout.
    tavolo->giocatori = playerInit(&numGiocatori, AI); //salviamo il numero all'interno di numGiocatori per le funzioni successive.
    tavolo->numeroTurni = 1;
    tavolo->numGiocatori = numGiocatori;
    tavolo->turnoCorrente = rand()%numGiocatori;

    //Funzioni di shuffling e dealing del mazzo.
    mainDeck = buildDeck(STANZA, STANZE_N, stanze);
    second = buildDeck(ARMA, ARMI_N, armi);
    third = buildDeck(SOSPETTO, SOSPETTI_N, sospetti);

    //peschiamo dalla cima del mazzo.
    tavolo->soluzione.cima = mainDeck->cima;
    mainDeck->cima = mainDeck->cima->next;
    tavolo->soluzione.cima->next = second->cima;
    second->cima = second->cima->next;
    tavolo->soluzione.cima->next->next = third->cima;
    third->cima = third->cima->next;
    tavolo->soluzione.cima->next->next->next = NULL;
    // Aggiorniamo gli indici di carte.
    mainDeck->numCarte--;
    second->numCarte--;
    third->numCarte--;
    tavolo->soluzione.numCarte = CARD_TYPES;
    printf("Omicidio eseguito. RIP Djanni.\n");


    mainDeck = mergeDecks(mainDeck, mergeDecks(second, third)); //uniamo i tre mazzi in fila. Liberiamo *second e *third.
    mainDeck = shuffleDeck_improved(mainDeck, mainDeck->numCarte); //mescoliamo un'altra volta.

    tavolo->carteScoperte.numCarte = mainDeck->numCarte%numGiocatori;
    tavolo->carteScoperte.cima = DealCards(mainDeck, mainDeck->numCarte%numGiocatori);

    cartePerGiocatore = mainDeck->numCarte / numGiocatori;
    for (i = 0; i<numGiocatori; i++){
        tavolo->giocatori[i].mano.cima = DealCards(mainDeck, cartePerGiocatore);
        tavolo->giocatori[i].mano.numCarte = cartePerGiocatore;
    }
    if(!mainDeck->numCarte)
        printf("Carte distribuite correttamente. Tavolo pronto.\n");



    for(i=0; i<CARD_TYPES; i++){ //Indicizziamo le stringhe in un ordine preciso. Usato nella formulazione dell'ipotesi.
        for (j = 0; j< (i==STANZA?STANZE_N:ARMI_N); j++){
            switch(i){
                case STANZA:
                    stanze(j, tavolo->stringhe[i][j]);
                    break;
                case ARMA:
                    armi(j, tavolo->stringhe[i][j]);
                    break;
               default:
                    sospetti(j, tavolo->stringhe[i][j]);
                    break;
            }
        }
    }

    statInit(tavolo);
    logger("\nCominciata nuova partita.\n");

    return tavolo;

}

Tabellone* LoadBoard(char* filename){
    int i, j;
    int stanzaIndex, sospIndex, armaIndex;
    Giocatore *ultimoG;
    Carta *ultimaC, *codaC;
    char msgbuf[SBUF] = "\nCaricata partita da ";

    FILE* save = fopen(filename, "rb");
    if (!save){
        fprintf(stderr, "Errore nel caricamento del file\n"
        "Verificare il nome e la posizione del file di salvataggio.\n");
        return NULL;
    }
    else {
        stanzaIndex = sospIndex = armaIndex = 0;
        Tabellone *table = (Tabellone *) malloc(sizeof(Tabellone)); //seguendo il formato byte a byte delle specifiche.
        if (!table)
            exit(-1);

        copiaMoveset(table->layout); //Copia il moveset costante e non contenuto nel salvataggio.
        fread(table, 3, sizeof(int), save); //Scrive i primi 3 int da disco, incluso il numero di giocatori necessario dopo.
        table->giocatori = (Giocatore*) calloc((size_t)table->numGiocatori, sizeof(Giocatore)); //Allochiamo un vettore di dimensione variabile.
        if (!table->giocatori)
            exit(-1);
        for (i = 0; i < table->numGiocatori; i++) {
            ultimoG = &table->giocatori[i];
            fread(ultimoG, 1, STANDARD_STRLEN * sizeof(char) + 2 * sizeof(int), save);
            fread(&ultimoG->mano.numCarte, 1, sizeof(int), save);//Simile ai giocatori ma ogni carta è un nodo di LL
            ultimoG->mano.cima = NULL;
            for (j = 0; j < ultimoG->mano.numCarte; j++) {        //e come tale lo possiamo allocare individualmente
                ultimaC = (Carta *) malloc(sizeof(Carta));        //avendo il riferimento all'elemento successivo esplicito.
                if (!ultimaC)
                    exit(-1);
                fread(&ultimaC->tipo, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //Il salvataggio contiene l'ordine e i valori non di link.
                switch(ultimaC->tipo){ // Generiamo l'indicizzazione delle stringhe dalle stringhe del file di ingresso.
                    case ARMA:
                        strcpy(table->stringhe[ARMA][armaIndex], ultimaC->desc);
                        armaIndex++;
                        break;
                    case SOSPETTO:
                        strcpy(table->stringhe[SOSPETTO][sospIndex], ultimaC->desc);
                        sospIndex++;
                        break;
                    default:
                        break;
                }
                ultimaC->next = NULL;
                if (!ultimoG->mano.cima) {                                                        //Costruiamo i link della lista.
                    ultimoG->mano.cima = ultimaC;
                } else {                                                                          //Aggiungiamo in coda dalla second carta in poi.
                    codaC = ultimoG->mano.cima;                                                   //Preserviamo l'ordine con cui erano elencate.
                    while (codaC->next)
                        codaC = codaC->next;
                    codaC->next = ultimaC;
                }

            }

        }
        fread(&table->carteScoperte.numCarte, 1, sizeof(int), save); //Carichiamo le carte scoperte.
        table->carteScoperte.cima = NULL;
        for (i = 0; i < table->carteScoperte.numCarte; i++) {
            ultimaC = (Carta *) malloc(sizeof(Carta));
            if (!ultimaC)
                exit(-1);
            fread(ultimaC, 1, sizeof(tipoCarta) + STANDARD_STRLEN * sizeof(char), save);
            switch(ultimaC->tipo){ // Importante che l'accodamento delle stringhe sia consecutivo tra i vari dati. L'ordine lo è di meno.
                case ARMA:
                    strcpy(table->stringhe[ARMA][armaIndex], ultimaC->desc);
                    armaIndex++;
                    break;
                case SOSPETTO:
                    strcpy(table->stringhe[SOSPETTO][sospIndex], ultimaC->desc);
                    sospIndex++;
                    break;
                default:
                    break;
            }
            if (!table->carteScoperte.cima) {
                table->carteScoperte.cima = ultimaC;
                ultimaC->next = NULL;
            } else {
                codaC = table->carteScoperte.cima;
                while (codaC->next)
                    codaC = codaC->next;
                codaC->next = ultimaC;
                ultimaC->next = NULL;
            }

        }
        table->soluzione.numCarte = CARD_TYPES; //le carte segrete sono hardcoded per essere 3(o perlomeno una per tipo).
        table->soluzione.cima = NULL;
        for (i = 0; i < table->soluzione.numCarte; i++) {
            ultimaC = (Carta *) malloc(sizeof(Carta));
            if (!ultimaC)
                exit(-1);
            fread(ultimaC, 1, sizeof(tipoCarta) + STANDARD_STRLEN * sizeof(char), save);
            switch(ultimaC->tipo){
                case ARMA:
                    strcpy(table->stringhe[ARMA][armaIndex], ultimaC->desc);
                    armaIndex++;
                    break;
                case SOSPETTO:
                    strcpy(table->stringhe[SOSPETTO][sospIndex], ultimaC->desc);
                    sospIndex++;
                    break;
                default:
                    break;
            }
            if (!table->soluzione.cima) {
                table->soluzione.cima = ultimaC;
                ultimaC->next = NULL;
            } else {
                codaC = table->soluzione.cima;
                while (codaC->next)
                    codaC = codaC->next;
                codaC->next = ultimaC;
                ultimaC->next = NULL;
            }
        }
        fclose(save);

        for(; stanzaIndex< STANZE_N; stanzaIndex++){
            stanze(stanzaIndex, table->stringhe[STANZA][stanzaIndex]); // E' importante che l'indice delle stanze sia consistente. Le stringhe devono essere consistenti da specifiche.
        }

        statInit(table); // Carichiamo le statistiche da file.

        strcat(msgbuf, filename);
        strcat(msgbuf, "\n");
        logger(msgbuf);


        return table;
    }

}

void FreeBoard(Tabellone* tavolo){
    int i, j;
    Carta *card1, *card2;

    //Liberiamo le carte sul tavolo, scoperte e soluzione.
    card1 = tavolo->soluzione.cima;
    for (i = 0; i < tavolo->soluzione.numCarte; i++) {
        card2 = card1->next;
        free(card1);
        card1 = card2;
    }
    card1 = tavolo->carteScoperte.cima;
    for (i = 0; i < tavolo->carteScoperte.numCarte; i++) {
        card2 = card1->next;
        free(card1);
        card1 = card2;
    }
    //Liberiamo le carte in mano a ogni giocatore.
    for (i=0; i<tavolo->numGiocatori; i++){
        card1 = tavolo->giocatori[i].mano.cima;
        for (j = 0; j < tavolo->giocatori[i].mano.numCarte; j++) {
            card2 = card1->next;
            free(card1);
            card1 = card2;
        }
    }
    //Liberiamo l'array dei giocatori. Questo ci libera anche dei mazzi interni alle loro strutture.
    free(tavolo->giocatori);
    //Liberiamo il tavolo e con lui i mazzi che abbiamo liberato per primi.
    free(tavolo);
}

void MainGame(Tabellone* tavolo, _Bool(*turnType)(Tabellone*, Giocatore*), _Bool AI){
    _Bool winner = 0;
    int i;
    char buf[SBUF];
    float loadArea[CARD_TYPES][STANZE_N];

    if(AI){ //Al caricamento della partita azzeriamo la memoria. Evita situazioni spiacevoli come credere in una mano sbagliata.
        for (i=0; i<tavolo->numGiocatori; i++){
            initInterest(tavolo, &tavolo->giocatori[i], loadArea);
            saveInterest(&tavolo->giocatori[i], loadArea);
            showingStrategy(&tavolo->giocatori[i], NULL, 0);
        }
        statShow();
    }


    while(!winner){
        winner = turnType(tavolo, &tavolo->giocatori[tavolo->turnoCorrente]);
        tavolo->turnoCorrente = (tavolo->turnoCorrente+1)%tavolo->numGiocatori;
        tavolo->numeroTurni +=1;
    }
    strncpy(buf, tavolo->giocatori[tavolo->turnoCorrente].nome, STANDARD_STRLEN);
    strcat(buf, " ha vinto.\n");
    logger(buf);
    printf("                                 _         _           _             _ _ \n"
           "  ___ ___  _ __   __ _ _ __ __ _| |_ _   _| | __ _ ___(_) ___  _ __ (_) |\n"
           " / __/ _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` |_  / |/ _ \\| '_ \\| | |\n"
           "| (_| (_) | | | | (_| | | | (_| | |_| |_| | | (_| |/ /| | (_) | | | | |_|\n"
           " \\___\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_/___|_|\\___/|_| |_|_(_)\n"
           "                 |___/                                                   \n"
           "\n%s ha vinto!\n"
           "Djanni può finalmente riposare in pace.\n\n"
           "...fino al gioco dell'anno prossimo.\n", tavolo->giocatori[tavolo->turnoCorrente].nome);

    statTrack(tavolo);
    statSave(tavolo);

    logger("Partita finita.\n");
}

_Bool Turn(Tabellone* tavolo, Giocatore* giocatore){
    int dice[2], i;
    Taccuino tac;
    Carta ipotesi[3]; // Ipotesi salvata in struttura carta. Buffer temporaneo per stringhe della decisione.
    char buf[SBUF], *pos;
    _Bool reachable[STANZE_N];
    // Inizializziamo i tipi della ipotesi.
    ipotesi[STANZA].tipo = STANZA;
    ipotesi[ARMA].tipo = ARMA;
    ipotesi[SOSPETTO].tipo = SOSPETTO;

    tac = IntroLines(tavolo, 0); //Carichiamo il taccuino in memoria temporanea, allochiamo dinamicamente i dati delle sue carte.


    if (giocatore->ipotesiEsatta){
        printf("Hai già compiuto l'ipotesi esatta.\n"
               "Procedi direttamente al lancio dei dadi.\n");
        strcpy(buf, "IL giocatore sta cercando di fare dadi doppi.\n");
        logger(buf);
        rollDice(dice);
        if (dice[0] == dice[1]){
            printf("Dadi doppi.\n");
            return 1;
        }
        else{
            printf("Dadi spaiati. Turno finito.\n\n");
            return 0;
        }
    }
    else{
        printf("Posizione attuale: %s\n", stanze(giocatore->stanza, ipotesi[STANZA].desc));
        printf("Vuoi procedere direttamente con l'ipotesi o lanciare i dadi?\n");
        do{
            printf("1 - Lancio dadi\n"
                           "2 - Ipotesi diretta\n ");
            inputWrapper(buf, &pos);
            dice[0] = strtol(buf, NULL, 10); //La variabile non è stata ancora usata in questo scopo.
        }while(dice[0] !=1 && dice[0] !=2);
        if (dice[0] == 1){
            rollDice(dice);
            dice[0] = dice[0] + dice[1]; //non abbiamo bisogno di conservare il valore dei singoli dadi.
            validPaths(tavolo->layout[giocatore->stanza], dice[0], reachable);
            printf("Movimenti validi:\n");
            for(dice[0] = 0, dice[1] = 0; dice[0] < STANZE_N; dice[0]++){
                if(reachable[dice[0]]){
                    printf("%d - %s(%d passi)\n", dice[0] + 1, tavolo->stringhe[STANZA][dice[0]], tavolo->layout[giocatore->stanza][dice[0]]);
                    dice[1]++;
                }
            }
            //Decisione spostamento se disponibile
            if(dice[1] > 1){
                printf("\nIn quale stanza desideri muoverti?\n");
                inputWrapper(buf, &pos);
                dice[1] = strtol(buf, NULL, 10) - 1;
                while(dice[1] >= STANZE_N || dice[1] < 0 ||!reachable[dice[1]]){ //short circuit ci permette di mettere la terza cond.
                    printf("Posizione non raggiungibile. Inserire stanza ammessa.\n");
                    inputWrapper(buf, &pos);
                    dice[1] = strtol(buf, NULL, 10) - 1;
                }
                strcpy(ipotesi[STANZA].desc, tavolo->stringhe[STANZA][dice[1]]);
                giocatore->stanza = dice[1];

                strcpy(buf, "Giocatore spostato in ");
                strcat (buf,  ipotesi[STANZA].desc);
                strcat(buf, "\n");
                printf(buf);
                logger(buf);
            }else {
                strcpy(ipotesi[STANZA].desc, tavolo->stringhe[STANZA][giocatore->stanza]);
                printf("Giocatore obbligato a rimanere in %s\n", ipotesi[STANZA].desc);
            }
        }
        //Formulazione ipotesi
        printf("L'ipotesi è obbligata a svolgersi nella tua stanza attuale(%s).\n", ipotesi[STANZA].desc);


        printf("Con quale arma si è compiuto il delitto?\n");
        for(dice[0] = 0; dice[0]<ARMI_N; dice[0]++){
            printf("%d - %s\n", dice[0] + 1, tavolo->stringhe[ARMA][dice[0]]);
        }
        inputWrapper(buf, &pos);
        dice[0] = strtol(buf, NULL, 10) - 1;
        while(dice[0] >= ARMI_N || dice[0] < 0 ){
            printf("Valore non ammesso. Inserire un numero adeguato.\n");
            inputWrapper(buf, &pos);
            dice[0] = strtol(buf, NULL, 10) - 1;
        }
        strcpy(ipotesi[ARMA].desc, tavolo->stringhe[ARMA][dice[0]]);

        printf("Quale sospetto ha compiuto il delitto?\n");
        for(dice[1] = 0; dice[1]<ARMI_N; dice[1]++){
            printf("%d - %s\n", dice[1] + 1, tavolo->stringhe[SOSPETTO][dice[1]]);
        }
        inputWrapper(buf, &pos);
        dice[1] = strtol(buf, NULL, 10) - 1;
        while(dice[1] >= SOSPETTI_N || dice[1] < 0 ){
            printf("Valore non ammesso. Inserire un numero adeguato.\n");
            inputWrapper(buf, &pos);
            dice[1] = strtol(buf, NULL, 10) - 1;
        }
        strcpy(ipotesi[SOSPETTO].desc, tavolo->stringhe[SOSPETTO][dice[1]]);

        for(i = 0; i<CARD_TYPES; i++){ //Aggiungiamo l'ipotesi al taccuino di questo turno.
            aggiornaTaccuino(&tac, ipotesi[i]);
        }
        giocatore->ipotesiEsatta = checkSolution(ipotesi[STANZA].desc, ipotesi[ARMA].desc, ipotesi[SOSPETTO].desc, tavolo, 0, NULL, &tac); //Controllo ipotesi. Ritorna int 0 - 1.

        if(giocatore->ipotesiEsatta){
            printf("Ipotesi esatta!\n"
                           "Per vincere, ottieni dadi doppi in uno dei prossimi turni.\n");
        }
        strcpy(buf, "Turno finito.\n\n");
        printf(buf);
        logger(buf);
        scriviTaccuino(giocatore->nome, &tac); //Il file di taccuino è salvato, le carte deallocate e il taccuino stesso deallocato alla fine dello scope.
        return 0;
    }
}

_Bool Turn_AI(Tabellone* tavolo, Giocatore* giocatore){ //Control flow più aderente possibile alla partita tradizionale, ma con feature che è meglio separare accuratamente.
    int dice[D_N], i, j, k;
    _Bool reachable[STANZE_N];
    Taccuino tac;
    Carta ipotesi[3];
    char buf[SBUF]; // buffer per linee da stampare.
    //Inizializziamo il tipo delle ipotesi.
    ipotesi[STANZA].tipo = STANZA;
    ipotesi[ARMA].tipo = ARMA;
    ipotesi[SOSPETTO].tipo = SOSPETTO;

    float interest[CARD_TYPES][STANZE_N]; //parte della logica decisionale AI

    readInterest(tavolo, giocatore, interest); // Carica la matrice esistente nell'interesse del giocatore corrente.
    tac = IntroLines(tavolo, 1); //Inizializzazione taccuino temporaneo.

    if (giocatore->ipotesiEsatta){ //Non c'è interazione in questa branch. Rimane identica alla versione umana.
        strcpy(buf, "Il giocatore sta cercando di fare dadi doppi.\n");
        printf("%s", buf);
        logger(buf);
        rollDice(dice);
        if (dice[0] == dice[1]){
            printf("Dadi doppi.\n");
            return 1;
        }
        else{
            printf("Dadi spaiati. Turno finito.\n\n");
            return 0;
        }
    }
    else{
        printf("Posizione attuale: %s\n", stanze(giocatore->stanza, ipotesi[STANZA].desc));
        printf("Procedere al lancio dei dadi.\n"); //La scelta ottimale per l'AI (ma anche per i giocatori umani) è sempre di valutare le opzioni di spostamento ed eventualmente stare fermi.
        rollDice(dice);
        dice[0] = dice[0] + dice[1]; //non abbiamo bisogno di conservare il valore dei singoli dadi.
        validPaths(tavolo->layout[giocatore->stanza], dice[0], reachable);
        printf("Movimenti validi:\n");
        for(dice[0] = 0, dice[1] = 0; dice[0] < STANZE_N; dice[0]++){
            if(reachable[dice[0]]){
                printf("%d - %s(%d passi)\n", dice[0] + 1, tavolo->stringhe[STANZA][dice[0]], tavolo->layout[giocatore->stanza][dice[0]]);
                dice[1]++;
            }
        }
        //Decisione spostamento se disponibile
        if(dice[1] > 1){
            dice[1] = movementStrategy(interest[2], reachable, tavolo->layout);
            printf("L'AI ha scelto l'opzione %d\n", dice[1] + 1);
            if(dice[1] >= STANZE_N || dice[1] < 0 ||!reachable[dice[1]]){ //short circuit ci permette di mettere la terza cond.
                fprintf(stderr, "%d: OUT OF INDEX\n", dice[1]); //L'AI non sbaglia, specie con l'accesso alla mask reachable. se atterriamo qua c'è da fare debugging.
                exit(-3);
            }
            strcpy(ipotesi[STANZA].desc, tavolo->stringhe[STANZA][dice[1]]);
            giocatore->stanza = dice[1];

            strcpy(buf, "Giocatore spostato in ");
            strcat (buf, ipotesi[STANZA].desc);
            strcat(buf, "\n");
            printf(buf);
            logger(buf);

        }else {
            strcpy(ipotesi[STANZA].desc, tavolo->stringhe[STANZA][giocatore->stanza]);
            printf("Giocatore obbligato a rimanere in %s\n", ipotesi[STANZA].desc);
        }

        //Formulazione ipotesi
        printf("L'ipotesi è obbligata a svolgersi nella tua stanza attuale(%s).\n", ipotesi[STANZA].desc);

        printf("Armi disponibili per l'ipotesi:\n");
        for(dice[0] = 0; dice[0]<ARMI_N; dice[0]++){
            printf("%d - %s\n", dice[0] + 1, tavolo->stringhe[ARMA][dice[0]]);
        }
        dice[0] = suspectStrategy(interest[ARMA]);
        printf("L'AI ha scelto l'opzione %d\n", dice[0] + 1);
        if(dice[0] >= ARMI_N || dice[0] < 0 ){ //come nelle stanze, questa opzione è qua per la visualizzazione d'errore. Normalmente è catturato in SuspectStrategy con -4.
            fprintf(stderr, "%d: OUT OF INDEX\n", dice[1]);
            exit(-3);
        }
        strcpy(ipotesi[ARMA].desc, tavolo->stringhe[ARMA][dice[0]]);

        printf("Sospetti disponibili per l'ipotesi:\n");
        for(dice[1] = 0; dice[1]<ARMI_N; dice[1]++){
            printf("%d - %s\n", dice[1] + 1, tavolo->stringhe[SOSPETTO][dice[1]]);
        }
        dice[1] = suspectStrategy(interest[SOSPETTO]);
        printf("L'AI ha scelto l'opzione %d\n", dice[1] + 1);
        if(dice[1] >= SOSPETTI_N || dice[1] < 0 ){
            fprintf(stderr, "%d: OUT OF INDEX\n", dice[1]);
            exit(-3);
        }
        strcpy(ipotesi[SOSPETTO].desc, tavolo->stringhe[SOSPETTO][dice[1]]);
        // Salviamo le carte ipotesi in coda sul taccuino.
        for(i = 0; i<CARD_TYPES; i++){
            aggiornaTaccuino(&tac, ipotesi[i]);
        }

        giocatore->ipotesiEsatta = checkSolution(ipotesi[STANZA].desc, ipotesi[ARMA].desc, ipotesi[SOSPETTO].desc, tavolo, 1, interest, &tac); //Passiamo le stringhe corrispondenti alle opzioni.
        if(giocatore->ipotesiEsatta){
            printf("Ipotesi esatta!\n"
                           "Per vincere, ottieni dadi doppi in uno dei prossimi turni.\n\n");
            // Se un giocatore fa l'ipotesi esatta tutte le AI ne prendono nota.
            // Crea la matrice di interesse adeguata.
            for(j=0; j<CARD_TYPES; j++){
                for(k=0; k< STANZE_N; k++){
                    interest[j][k] = 0.0f;
                }
            }
            interest[STANZA][giocatore->stanza] = 1.0f;
            interest[ARMA][dice[0]] = 1.0f;
            interest[SOSPETTO][dice[1]] = 1.0f;
            // Sovrascrive le matrici dei giocatori.
            for(i=0; i<tavolo->numGiocatori; i++){
                saveInterest(&tavolo->giocatori[i], interest);
            }
        }else{
            /*
             * C'è una possibilità statistica non indifferente che le carte parte di una soluzione sbagliata ma non
             * trovate nella mano di nessuno possano non essere state chiamate perchè tra le carte segrete.
             * Questa sezione aggiusta un bias per le carte chiamate più volte e mai contestate.
             * La carta trovata è stata in precedenza azzerata.
             * Non c'è pericolo di overflow perchè 1.5^(STANZE_N*ARMI_N) ~ 3e+9 << FLT_MAX.
             */
            interest[STANZA][giocatore->stanza]*=INTEREST_FACTOR;
            interest[ARMA][dice[0]]*=INTEREST_FACTOR;
            interest[SOSPETTO][dice[1]]*=INTEREST_FACTOR;
            saveInterest(giocatore, interest);

        }
        printf("Turno finito.\n\n");
        scriviTaccuino(giocatore->nome, &tac); //Salvataggio taccuino su disco. Deallocazione delle carte taccuino di questo turno.
        return 0;
    }
}

Taccuino IntroLines(Tabellone* tavolo, _Bool AI){ //Passiamo valori di taccuino e indirizzi delle carte caricate.
    char buf[SBUF], turnobuf[STANDARD_STRLEN], *pos;
    FILE* test;
    Taccuino tac;

    strcpy(buf, "\nTURNO ");
    strcat(buf,  dtoc(tavolo->numeroTurni, turnobuf));
    strcat(buf, " - Giocatore: ");
    strcat(buf, tavolo->giocatori[tavolo->turnoCorrente].nome);
    strcat(buf, "\n");
    printf(buf);
    logger(buf);

    printf("\nVuoi salvare lo stato attuale della partita? S/N\n");
    inputWrapper(buf, &pos);
    if(tolower(buf[0]) == 's'){
        do {
            printf("Inserire il nome del file in cui salvare.(max %d caratteri)\n", STANDARD_STRLEN - 1);
            inputWrapper(buf, &pos);
            if((test = fopen(buf, "wb")) == NULL){
                fprintf(stderr, "Errore nella creazione del salvataggio.(Caratteri non supportati)\n");
            }else{
                fclose(test);
            }
        }while(!test);

        saveState(buf, tavolo);

        logger("Partita salvata in ");
        logger(buf);
        logger("\n");

        printf("Vuoi uscire dalla partita? S/N\n");
        inputWrapper(buf, &pos);
        if(tolower(buf[0]) == 's'){
            printf("\nArrivederci!\n");
            logger("Programma terminato.\n");
            exit(0);
        }
    }
    printTableStatus(tavolo, AI);
    tac = leggiTaccuino(tavolo->giocatori[tavolo->turnoCorrente].nome);

    if(!AI){
        parseTac(&tac);
    }

    return tac;
}