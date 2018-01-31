//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Gameplay.h"

#define D_SIDES 6 //giusto in caso si voglia giocare con dadi diversi.

/* NOTA SUL TACCUINO
 * Il taccuino non ha alcun dato di identificazione se non il nome dele giocatore.
 * Cominciare una partita con lo stesso nome giocatore riscrive il contenuto del taccuino.
 * Continuare una partita precedente dopo tale evento comunica informazioni incorrette
 * sulla propria mano e il tavolo in console (il tavolo è e rimane intoccato).
 */

void leggiTaccuino(char* filename){
    FILE* tac;
    char buf[STANDARD_STRLEN + 4], *sbuffer;
    int linebuf = SBUF;

    strcpy(buf, filename);
    strcat(buf, ".tac");

    tac = fopen(buf, "r");
    if(!tac){
        exit(-2);
    }
    sbuffer = (char*)malloc(SBUF*sizeof(char)); //la misura non conta molto, getline espande il buffer con realloc se necessario.
    while(!feof(tac)){
        getline(&sbuffer, (size_t*)&linebuf, tac);
        printf("%s\n", sbuffer);
    }
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

void rollDice(int dice[2]){
    char msgbuf[2][STANDARD_STRLEN];
    int i;

    logger("Dadi tirati.\n");
    for(i=0; i<2; i++) {
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
        if (layout[i]<= val)
            out[i] = 1;
        else
            out[i] = 0;
    }
}

int checkSolution(const char* stanza,const char* arma,const char* sospetto, Tabellone* tavolo){
    Carta* matching;
    int found = 0;
    Carta foundData[3];
    char message[SBUF] ="L'ipotesi del giocatore è:\t";
    int i, j;

    strncat(message, stanza, strlen(stanza));
    strcat(message, "\t");
    strncat(message, arma, strlen(arma));
    strcat(message, "\t");
    strncat(message, sospetto, strlen(sospetto));
    strcat(message, "\n");
    logger(message);

    //controlla il tavolo
    matching = tavolo->carteScoperte.cima;
    for ( i=0; i<tavolo->carteScoperte.numCarte; i++){
        if(checkCard(stanza, arma, sospetto, matching)){ //non è importante far decidere quale mostrare qua.
            strcpy(message, "Carta trovata sul tavolo!");
            printf(message);
            logger(message);
            tipi(matching->tipo, message);
            strcat(message, " - ");
            strcat(message,  matching->desc);
            strcat(message, "\n");
            logger(message);
            //Se la carta è sul tavolo si trova già nel taccuino.
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
                          matching)) { //cambiamo il tipo di broadcast pubblico per permettere la scelta di cosa mostrare.
                foundData[found] = *matching;
                found++;
            }
            if (matching->next)
                matching = matching->next;
        }
        if (found>1){
            printf("MESSAGGIO PRIVATO PER %s\n",  tavolo->giocatori[i%tavolo->numGiocatori].nome);
            printf("Più carte dell' ipotesi sono nella tua mano. Decidi quale mostrare.\n");
            for (j=0; j<found; j++){
                printf("%d: %s - %s\n", j, tipi(foundData[j].tipo, message), foundData[j].desc); //usiamo message come buffer temporaneo per la stringa di ritorno.
            }
            scanf("%d", &j);
            while(j<0 || j>=found){
                printf("Numero invalido. Reinserire.\n");
                scanf("%d", &j);
            }
            foundData[0] = foundData[j]; //sovrascriviamo e stampiamo solo il primo elemento.
        }
        if(found>0){ //effettiva comunicazione ai giocatori. Registrazione nel taccuino.
            printf("\nIpotesi errata.\n");
            printf("MESSAGGIO PRIVATO PER %s\n", tavolo->giocatori[tavolo->turnoCorrente].nome);
            strcpy(message,"Carta trovata nella mano di ");
            strcat(message, tavolo->giocatori[i%tavolo->numGiocatori].nome);
            strcat(message, ".\n");
            logger(message);
            printf(message);

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
    return 0;
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