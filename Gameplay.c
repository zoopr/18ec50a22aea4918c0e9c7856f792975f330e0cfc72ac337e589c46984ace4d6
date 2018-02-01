//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Gameplay.h"
#include "Ai.h"


void leggiTaccuino(char* filename){
    FILE* tac;
    char buf[STANDARD_STRLEN + 4], *sbuffer;
    int linebuf = SBUF;

    strcpy(buf, filename);
    strcat(buf, ".tac");

    tac = fopen(buf, "r");
    if(tac){
        sbuffer = (char*)malloc(SBUF*sizeof(char)); //la misura non conta molto, getline espande il buffer con realloc se necessario.
        while(!feof(tac)){
            getline(&sbuffer, (size_t*)&linebuf, tac);
            printf("%s\n", sbuffer);
        }
    }else{
        tac = fopen(buf, "a+"); // Crea un taccuino se non lo trova. Utile caricando salvataggi con nomi giocatore non presenti.
        if (!tac) //Se pure la creazione automatica fallisce c'è chiaramente un errore di scala più grande del programma.
            exit(-2);
    }
    fclose(tac);
}

void scriviTaccuino(char* filename, char* message){
    FILE* tac;
    char buf[STANDARD_STRLEN + 4];
    strcpy(buf, filename);
    strcat(buf, ".tac");

    tac = fopen(buf, "a");
    if (!tac){
        exit(-2);
    }
    fprintf(tac, message);
    fclose(tac);
}

void rollDice(int dice[D_N]){
    char msgbuf[D_N][STANDARD_STRLEN];
    int i;

    logger("Dadi tirati.\n");
    for(i=0; i<D_N; i++) {
        dice[i] = rand() % D_SIDES + 1;
        dtoc(dice[i], msgbuf[i]);
    }

    printf("Hai fatto %d + %d\n", dice[0], dice[1]);
    strcat(msgbuf[0], " + ");
    strcat(msgbuf[0], msgbuf[1]);
    strcat(msgbuf[0], "\n");
    logger(msgbuf[0]);
}

void validPaths(const int layout[STANZE_N], int val, _Bool out[STANZE_N]){
    int i;
    for (i=0; i<STANZE_N; i++){
        if (layout[i] <= val)
            out[i] = 1;
        else
            out[i] = 0;
    }
}

int checkSolution(const char* stanza,const char* arma,const char* sospetto, Tabellone* tavolo, _Bool AI, float interestFile[CARD_TYPES][STANZE_N]){
    Carta* matching;
    Carta foundData[CARD_TYPES];
    int i, j, coords[CARD_TYPES]; //Usiamo i campi per l'indice nella lista di chi ha le carte, ed i primi due campi per generare coordinate per chi has sbagliato.
    int found = 0;
    char message[SBUF] ="L'ipotesi del giocatore è:\t";


    strncat(message, stanza, strlen(stanza));
    strcat(message, "\t");
    strncat(message, arma, strlen(arma));
    strcat(message, "\t");
    strncat(message, sospetto, strlen(sospetto));
    strcat(message, "\n");
    logger(message);
    printf(message);


    matching = tavolo->carteScoperte.cima; // Mai raggiungibile dall'AI a meno che non si muova qua casualmente quando nessuna stanza d'interesse è disponibile.
    for ( i=0; i<tavolo->carteScoperte.numCarte; i++){
        if(checkCard(stanza, arma, sospetto, matching) != -1){ //non è importante far decidere quale carta mostrare quando sono già sul tavolo.
            strcpy(message, "Carta trovata sul tavolo.\n");
            printf(message);
            logger(message);
            tipi(matching->tipo, message);
            strcat(message, " - ");
            strcat(message,  matching->desc);
            strcat(message, "\n");
            logger(message);
            //No printf: Se la carta è sul tavolo viene stampata di default a ogni turno.
            return 0;
        }
        if (matching->next)
            matching = matching->next;
    }

    //controlla le mani dei giocatori in avanti fino al giocatore incluso.
    for (i=tavolo->turnoCorrente; i<(tavolo->turnoCorrente + tavolo->numGiocatori) && !found; i++) {
        matching = tavolo->giocatori[i%tavolo->numGiocatori].mano.cima;
        for (j = 0; j < tavolo->giocatori[i%tavolo->numGiocatori].mano.numCarte; j++){
            if (checkCard(stanza, arma, sospetto,
                          matching) != -1) { //cambiamo il tipo di broadcast pubblico per permettere la scelta di cosa mostrare.
                foundData[found] = *matching;
                coords[found] = j;
                found++;
            }
            if (matching->next)
                matching = matching->next;
        }
        if (found>1){
            printf("MESSAGGIO PRIVATO PER %s\n",  tavolo->giocatori[i%tavolo->numGiocatori].nome);
            printf("Più carte dell' ipotesi sono nella tua mano. Decidi quale mostrare.\n");
            for (j=0; j<found; j++){
                printf("%d: %s - %s\n", j + 1, tipi(foundData[j].tipo, message), foundData[j].desc); //usiamo message come buffer temporaneo per la stringa di ritorno.
            }
            if (AI){
                j = showingStrategy(&tavolo->giocatori[i%tavolo->numGiocatori], coords, found);
                printf("L'AI ha scelto l'opzione %d\n", j + 1);
            }else{
                scanf("%s", message);
                j = strtol(message, NULL, 10) - 1;
            }
            while(j<0 || j>=found){ //Con la flag AI non dovrebbe mai entrare in questo branch.
                printf("Numero invalido. Reinserire.\n");
                scanf("%s", message);
                j = strtol(message, NULL, 10) - 1;
            }
            foundData[0] = foundData[j]; //sovrascriviamo e stampiamo solo il primo elemento.
        }
        if(found>0){ //effettiva comunicazione ai giocatori. Registrazione nel taccuino.
            if (AI && found == 1){//Dobbiamo comunque aggiornare che carta abbiamo dovuto mostrare al giocatore corrente
                showingStrategy(&tavolo->giocatori[i%tavolo->numGiocatori], coords, found);
            }
            printf("\nIpotesi errata.\n");
            printf("MESSAGGIO PRIVATO PER %s\n", tavolo->giocatori[tavolo->turnoCorrente].nome);
            strcpy(message,"Carta trovata nella mano di ");
            strcat(message, tavolo->giocatori[i%tavolo->numGiocatori].nome);
            strcat(message, ".\n");
            logger(message);
            printf(message);

            if(AI){
                generateCoordinates(foundData, coords);
                interestFile[coords[0]][coords[1]] = 0.0f;
            }

            tipi(foundData[0].tipo, message);
            strcat(message, " - ");
            strcat(message, foundData[0].desc);
            strcat(message, "\n");
            logger(message);
            printf("La carta mostrata è %s\n", message);
            scriviTaccuino(tavolo->giocatori[tavolo->turnoCorrente].nome, message);// La carta è comunicata esclusivamente al taccuino del giocatore.
            return 0;

        }
    }
    strcpy(message, "Ipotesi esatta.\n");
    //l'AI perde completamente il controllo del game flow da qui in poi.
    logger(message);
    return 1;//è tra le carte segrete.

}

int checkCard(const char* stanza,const char* arma,const char* sospetto, Carta* card){
    if (strcmp(stanza, card->desc) == 0){
        return 1;
    }
    else if(strcmp(arma, card->desc) == 0){
        return 2;
    }
    else if(strcmp(sospetto, card->desc) == 0){
        return 3;
    }
    return -1;
}

void saveState(char* filename, Tabellone* board){
    int i, j;
    Carta* carta;
    char msgbuf[SBUF] = "\nPartita salvata in ";
    FILE* save = fopen(filename, "w"); //qualunque cosa ci sia la sovrascrive.
    if (!save)
        exit(-2);


    fwrite(&board->turnoCorrente, 1, sizeof(int), save); //scrive turnoCorrente, numeroTurni, numeroGiocatori.
    fwrite(&board->numeroTurni, 1, sizeof(int), save);
    fwrite(&board->numGiocatori, 1, sizeof(int), save);

    for (i=0; i< board->numGiocatori; i++){
        fwrite(&board->giocatori[i].nome, 1, STANDARD_STRLEN*sizeof(char), save); //scrive la prima parte dei dati giocatore
        fwrite(&board->giocatori[i].stanza, 1, sizeof(int), save);
        fwrite(&board->giocatori[i].ipotesiEsatta, 1, sizeof(int), save);
        fwrite(&board->giocatori[i].mano.numCarte, 1, sizeof(int), save); // copia il numero di carte
        carta = board->giocatori[i].mano.cima;
        for (j=0; j<board->giocatori[i].mano.numCarte; j++){
            fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //salva la prima parte della carta.
            if (carta->next) //scorre fino all'ultima.
                carta = carta->next;
        }

    }
    fwrite( &board->carteScoperte.numCarte, 1, sizeof(int), save); //sezione identica al salvataggio consecutivo delle carte giocatore.
    carta = board->carteScoperte.cima;
    for (j=0; j<board->carteScoperte.numCarte; j++){
        fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //salva la prima parte della carta.
        if (carta->next) //scorre fino all'ultima.
            carta = carta->next;
    }
    carta = board->soluzione.cima; //ovviamente l'ultimo blocco di carte non ha un int esplicito di lunghezza.
    for (j=0; j<board->soluzione.numCarte; j++){
        fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //salva la prima parte della carta.
        if (carta->next) //scorre fino all'ultima.
            carta = carta->next;
    }
    fclose(save);
    // composizione del messaggio di logging.
    strcat(msgbuf, filename);
    strcat(msgbuf, "\n\n");
    logger(msgbuf);
}

void logger(char* message){
    FILE* logger = fopen(LOG_DEFAULT, "a");
    if(!logger)
        exit(-2);

    fprintf(logger, message);
    fclose(logger);
}

void statInit(Tabellone* tavolo){ //carica o eventualmente crea il file di statistiche e lo carica nel tavolo.
    int i;
    FILE* tac;

    tac = fopen(STAT_DEFAULT, "r");
    if(tac && !feof(tac)){ // Ci assicura che nella prima partita del sistema sia gestito correttamente.
        fread(tavolo->stats, 3, STANZE_N*sizeof(int), tac);
        fclose(tac);
    }else{
        for(i=0; i<3*STANZE_N; i++){
            tavolo->stats[i/STANZE_N][i%STANZE_N] = 0;
        }
        statSave(tavolo); //Salviamo una versione azzerata se non troviamo le statistiche sul disco.
    }
}

void statSave(Tabellone* tavolo){
    FILE* tac;

    tac = fopen(STAT_DEFAULT, "w");
    if(!tac) {
        exit(-2);
    }
    fwrite(tavolo->stats, 3, STANZE_N*sizeof(int), tac);
    fclose(tac);
}

void statTrack(Tabellone* tavolo){
    Carta daValutare[CARD_TYPES];
    int valSoluzioni[CARD_TYPES];
    Carta* scroll = tavolo->soluzione.cima;
    int i;

    //salviamo le carte in ordine: stanza, arma, sospetto.
    for (i=0; i<3; i++){
        switch(scroll->tipo){
            case STANZA:
                daValutare[0] = *scroll;
                break;
            case ARMA:
                daValutare[1] = *scroll;
            default:
                daValutare[2] = *scroll;
        }
        if(scroll->next){
            scroll = scroll->next;
        }
    }
    // Estrapoliamo gli indici.
    valSoluzioni[STANZA] = checkCard_Archive(stanze, &daValutare[0], STANZE_N);
    valSoluzioni[ARMA] = checkCard_Archive(armi, &daValutare[1], ARMI_N);
    valSoluzioni[SOSPETTO] = checkCard_Archive(sospetti, &daValutare[2], SOSPETTI_N);

    //Aggiorniamo le statistiche caricate nel tavolo.
    for(i=0; i<CARD_TYPES; i++){
        tavolo->stats[i][valSoluzioni[i]]++;
    }
}

void statShow(){
    char buf[STANDARD_STRLEN];
    int statsArr[CARD_TYPES][STANZE_N], i;
    FILE* stats = fopen(STAT_DEFAULT, "r");
    if(!stats){
        printf("Nessun file di statistiche rilevato. Hai giocato ad almeno una partita?\n");
        return;
    }
    else{
        fread(statsArr, CARD_TYPES, STANZE_N*sizeof(int), stats);
        printf("\nStatistiche del crimine:\n\n");
        printf("STANZE\n");
        for(i=0;i<STANZE_N; i++){
            printf("%s - %d volte\n", stanze(i, buf), statsArr[0][i]);
        }
        printf("\nARMI\n");
        for(i=0;i<ARMI_N; i++){
            printf("%s - %d volte\n", armi(i, buf), statsArr[1][i]);
        }
        printf("\nSOSPETTI\n");
        for(i=0;i<SOSPETTI_N; i++){
            printf("%s - %d volte\n", sospetti(i, buf), statsArr[2][i]);
        }
        printf("\n");

    }
}

int checkCard_Archive(char* (*func)(int, char*), Carta* card, int len){ //riporta l'indice dell'elemento nel vettore.
    int i;
    char buf[SBUF];
    for(i=0; i<len; i++) {
        if (strcmp(func(i, buf), card->desc) == 0) {
            return i;
        }
    }
    return -1;
}