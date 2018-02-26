//
// Created by Mat on 18/01/30.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Gameplay.h"
#include "AI.h"


Taccuino leggiTaccuino(char* filename){ //Legge il taccuino da disco, alloca le carte adeguate e lo ritorna.
    FILE* tac;
    char buf[STANDARD_STRLEN + 4];
    Taccuino tacFile = {0, NULL}; //il nostro file interno.
    int i, j, errorChecking;
    Carta *carta, *coda;

    strcpy(buf, filename);
    strcat(buf, ".tac");

    tac = fopen(buf, "rb");
    if(tac){
        errorChecking = fread(&tacFile.numCarte, 1, sizeof(int), tac);
        if(!errorChecking){ //Taccuino di formato diverso o corrotto.
            wipeTac(buf);
            tacFile.numCarte = 0; // Reinizializza in caso di errore lettura parziale(numero di byte già letti e copiati < sizeof(int) prima di eof).
            return tacFile;
        }
        tacFile.cima = NULL;
        for(i=0; i<tacFile.numCarte; i++){
            carta = (Carta*)malloc(sizeof(Carta));
            if(!carta)
                exit(-1);
            errorChecking = fread(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), tac);
            if(!errorChecking){ //Taccuino con informazioni parziali. Non può essere fidato di mostrare le informazion adeguate.
                //Libera le carte caricate finora.
                for(j=0; j<=i; j++){
                    carta = tacFile.cima;
                    tacFile.cima = carta->next;
                    free(carta);
                }
                wipeTac(buf); //Pulisce il file.
                tacFile.numCarte = 0; // Reinizializza in caso di errore lettura parziale.
                tacFile.cima = NULL;
                return tacFile;
            }
            carta->next = NULL;
            //Aggiungiamo in coda.
            coda = tacFile.cima;
            if(!coda){
                tacFile.cima = carta;
            }else {
                while (coda->next) {
                    coda = coda->next;
                }
                coda->next = carta;
            }
        }
        fclose(tac);
        return tacFile;
    }else{
        wipeTac(buf);
        return tacFile;
    }
}

int wipeTac(char buf[STANDARD_STRLEN + 4]){
    FILE* tac;

    tac = fopen(buf, "wb"); // Crea un taccuino se non lo trova. Utile caricando salvataggi con nomi giocatore non presenti.
    if (!tac) { //Se pure la creazione automatica fallisce c'è chiaramente un errore di scala più grande del programma.
        fprintf(stderr, "Errore nella creazione di un file taccuino al nome %s. (Caratteri non supportati)\n", buf);
        return -2;
    }
    fclose(tac);
    return 0;
}

void aggiornaTaccuino(Taccuino* tac, Carta carta){ //Inserisamo una nuova carta in coda al taccuino.
    Carta* cartaTac, *coda;

    cartaTac = (Carta*)malloc(sizeof(Carta));
    if(!cartaTac)
        exit(-1);

    *cartaTac = carta;
    cartaTac->next = NULL;
    coda = tac->cima;
    if(!coda){
        tac->cima = cartaTac;
    }else{
        while(coda->next){
            coda = coda->next;
        }
        coda->next = cartaTac;
    }
    tac->numCarte++;
}

void scriviTaccuino(char* filename, Taccuino* tacFile){ //Salviamo il contenuto del taccuino a fine turno e deallochiamo le carte caricate al momento.
    FILE* tac;
    char buf[STANDARD_STRLEN + 4];
    int i;
    Carta* carta;

    strcpy(buf, filename);
    strcat(buf, ".tac");

    tac = fopen(buf, "wb");
    if (!tac){
        exit(-2);
    }
    fwrite(tacFile, 1, sizeof(int), tac);
    for(i=0; i<tacFile->numCarte; i++){ //Scriviamo e liberiamo le carte.
        carta = tacFile->cima;
        fwrite(carta, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), tac);
        tacFile->cima = carta->next;
        free(carta);
    }
    fclose(tac);
}

void rollDice(int dice[D_N]){ // D_N rand() salvati nella memoria a parametro d'ingresso.
    char msgbuf[D_N][STANDARD_STRLEN];
    int i;

    logger("Dadi tirati.\n");
    for(i=0; i<D_N; i++) {
        dice[i] = rand() % D_SIDES + 1;
        dtoc(dice[i], msgbuf[i]);
    }


    strcat(msgbuf[0], " + ");
    strcat(msgbuf[0], msgbuf[1]);
    strcat(msgbuf[0], "\n");
    logger(msgbuf[0]);
    printf("Hai fatto %s\n", msgbuf[0]); // Standard per 2 dadi.
}

void validPaths(const int layout[STANZE_N], int val, _Bool out[STANZE_N]){ // trova le stanze ragguingibili dalla riga di layout[][] data in ingresso.
    int i;
    for (i=0; i<STANZE_N; i++){
        if (layout[i] <= val)
            out[i] = 1;
        else
            out[i] = 0;
    }
}

int checkSolution(const char* stanza,const char* arma,const char* sospetto, Tabellone* tavolo, _Bool AI, float interestFile[CARD_TYPES][STANZE_N], Taccuino* tac){
    Carta* matching;
    Carta foundData[CARD_TYPES]; //Vettore temporaneo di tre carte per salvare i dati di qualsiasi delle tre carte d'ipotesi sia stata trovata.
    int i, j, coords[CARD_TYPES]; //Usiamo i campi per l'indice nella lista di chi ha le carte, ed i primi due campi per generare coordinate per chi has sbagliato.
    int found = 0;
    char message[SBUF] ="L'ipotesi del giocatore è:\t";
    char* pos;

    strncat(message, stanza, strlen(stanza));
    strcat(message, "\t");
    strncat(message, arma, strlen(arma));
    strcat(message, "\t");
    strncat(message, sospetto, strlen(sospetto));
    strcat(message, "\n");
    logger(message);
    printf(message);

    matching = tavolo->carteScoperte.cima;
    for ( i=0; i<tavolo->carteScoperte.numCarte; i++){// Mai raggiungibile dall'AI a meno che non si muova qua casualmente quando nessuna stanza d'interesse è disponibile.
        if(checkCard(stanza, arma, sospetto, matching) != -1){
            strcpy(message, "Carta trovata sul tavolo.\n"); // Non è importante far decidere quale mostrare se almeno una carta è sul tavolo.
            printf(message);
            logger(message);
            tipi(matching->tipo, message);
            strcat(message, " - ");
            strcat(message,  matching->desc);
            strcat(message, "\n");
            logger(message);
            // Se la carta è sul tavolo viene stampata di default a ogni turno. Nessun taccuino (e 0 interesse di default).
            return 0;
        }
        if (matching->next)
            matching = matching->next;
    }

    //controlla le mani dei giocatori in avanti fino al giocatore stesso.
    for (i=tavolo->turnoCorrente + 1; i<=(tavolo->turnoCorrente + tavolo->numGiocatori) && !found; i++) {
        matching = tavolo->giocatori[i%tavolo->numGiocatori].mano.cima;
        for (j = 0; j < tavolo->giocatori[i%tavolo->numGiocatori].mano.numCarte; j++){
            if (checkCard(stanza, arma, sospetto,
                          matching) != -1) { // Permettere la scelta di cosa mostrare.
                foundData[found] = *matching;
                coords[found] = j;
                found++;
            }
            if (matching->next)
                matching = matching->next;
        }
        if (found>1 && (i%tavolo->numGiocatori) != tavolo->turnoCorrente){ //Se più di una carta dell'ipotesi è un nostro bluff e le altre sono corrette non ci chiediamo di scegliere.
            printf("MESSAGGIO PRIVATO PER %s\n",  tavolo->giocatori[i%tavolo->numGiocatori].nome);
            printf("Più carte dell' ipotesi sono nella tua mano. Decidi quale mostrare.\n");
            for (j=0; j<found; j++){
                printf("%d: %s - %s\n", j + 1, tipi(foundData[j].tipo, message), foundData[j].desc);
            }
            if (AI){
                j = showingStrategy(&tavolo->giocatori[i%tavolo->numGiocatori], coords, found);
                printf("L'AI ha scelto l'opzione %d\n", j + 1);
            }else{
                inputWrapper(message, &pos);
                j = strtol(message, NULL, 10) - 1;
            }
            while(j<0 || j>=found){ //Con la flag AI non dovrebbe mai entrare in questo branch.
                printf("Numero invalido. Reinserire.\n");
                inputWrapper(message, &pos);
                j = strtol(message, NULL, 10) - 1;
            }
            foundData[0] = foundData[j]; //sovrascriviamo e stampiamo solo il primo elemento.

        }
        if(found>0){ //effettiva comunicazione ai giocatori. Registrazione nel taccuino.
            if (AI && found == 1 && (i%tavolo->numGiocatori) != tavolo->turnoCorrente){ //il giocatore che teneva una carta per fermare l'ipotesi aggiorna che carta ha dovuto mostrare al giocatore corrente.
                showingStrategy(&tavolo->giocatori[i%tavolo->numGiocatori], coords, found);
            }
            printf("\nIpotesi errata.\n");
            printf("MESSAGGIO PRIVATO PER %s\n", tavolo->giocatori[tavolo->turnoCorrente].nome);
            strcpy(message,"Carta trovata nella mano di ");
            strcat(message, tavolo->giocatori[i%tavolo->numGiocatori].nome);
            strcat(message, ".\n");
            logger(message);
            printf(message);

            if(AI){ //Registriamo che carta ci sia stata mostrata e facciamo in modo di non sceglierla mai più.
                generateCoordinates(tavolo, foundData, coords);
                interestFile[coords[0]][coords[1]] = 0.0f;
            }

            tipi(foundData[0].tipo, message);
            strcat(message, " - ");
            strcat(message, foundData[0].desc);
            strcat(message, "\n");
            logger(message);
            printf("La carta mostrata è %s\n", message);
             // La carta è aggiunta esclusivamente al taccuino del giocatore.
            aggiornaTaccuino(tac, foundData[0]);
            return 0;

        }
    }
    strcpy(message, "Ipotesi esatta.\n"); // Sia il giocatore che l'AI perdono completamente il controllo del game flow da qui in poi.
    logger(message);
    return 1;

}

int checkCard(const char* stanza,const char* arma,const char* sospetto, Carta* card){ // Riporta coincidenza tra carta e ipotesi e il suo tipo.
    if (strcmp(stanza, card->desc) == 0){
        return STANZA;
    }
    else if(strcmp(arma, card->desc) == 0){
        return ARMA;
    }
    else if(strcmp(sospetto, card->desc) == 0){
        return SOSPETTO;
    }
    return -1;
}

void saveState(char* filename, Tabellone* board){
    int i, j;
    Carta* carta;
    char msgbuf[SBUF] = "\nPartita salvata in ";
    FILE* save = fopen(filename, "wb"); //qualunque cosa ci sia la sovrascrive.
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

void logger(char* message){ // Scrive la stringa in ingresso sul logger. Estremamente general purpose.
    FILE* logger = fopen(LOG_DEFAULT, "a");
    if(!logger)
        exit(-2);

    fprintf(logger, message);
    fclose(logger);
}

void statInit(Tabellone* tavolo){ //carica o eventualmente crea il file di statistiche e lo carica nel tavolo.
    int i;
    FILE* stat;

    stat = fopen(STAT_DEFAULT, "rb");
    if(stat && !feof(stat)){ // Ci assicura che nella prima partita del sistema sia gestito correttamente.
        fread(tavolo->stats, CARD_TYPES, STANZE_N*sizeof(int), stat);
        fclose(stat);
    }else{
        for(i=0; i<CARD_TYPES*STANZE_N; i++){
            tavolo->stats[i/STANZE_N][i%STANZE_N] = 0;
        }
        statSave(tavolo); //Salviamo una versione azzerata se non troviamo le statistiche sul disco.
    }
}

void statSave(Tabellone* tavolo){ // Scrive le nuove statistiche a fine partita.
    FILE* stat;

    stat = fopen(STAT_DEFAULT, "wb");
    if(!stat) {
        exit(-2);
    }
    fwrite(tavolo->stats, 3, STANZE_N*sizeof(int), stat);
    fclose(stat);
}

void statTrack(Tabellone* tavolo){ // Aggiorna le statistiche precedentemente caricate da file.
    Carta daValutare[CARD_TYPES];
    int valSoluzioni[CARD_TYPES];
    Carta* scroll = tavolo->soluzione.cima;
    int i, j;
    char ourIndexes[CARD_TYPES][STANZE_N][STANDARD_STRLEN];

    for(i=0; i<CARD_TYPES; i++){ //Generiamo l' ordine delle carte per rimapparle correttamente.
        for (j = 0; j< (i==STANZA?STANZE_N:ARMI_N); j++){
            switch(i){
                case STANZA:
                    stanze(j, ourIndexes[i][j]);
                    break;
                case ARMA:
                    armi(j, ourIndexes[i][j]);
                    break;
                case SOSPETTO:
                    sospetti(j, ourIndexes[i][j]);
                    break;
            }
        }
    }
    // Salviamo le carte in ordine.
    for (i=0; i<CARD_TYPES; i++){
        switch(scroll->tipo){
            case STANZA:
                daValutare[STANZA] = *scroll;
                break;
            case ARMA:
                daValutare[ARMA] = *scroll;
                break;
            default:
                daValutare[SOSPETTO] = *scroll;
                break;
        }
        if(scroll->next){
            scroll = scroll->next;
        }
    }
    // Estrapoliamo gli indici.
    valSoluzioni[STANZA] = checkCard_Archive(ourIndexes, &daValutare[STANZA], STANZE_N);
    valSoluzioni[ARMA] = checkCard_Archive(ourIndexes, &daValutare[ARMA], ARMI_N);
    valSoluzioni[SOSPETTO] = checkCard_Archive(ourIndexes, &daValutare[SOSPETTO], SOSPETTI_N);
    // Aggiorniamo le statistiche caricate nel tavolo.
    for(i=0; i<CARD_TYPES; i++){
        tavolo->stats[i][valSoluzioni[i]]++;
    }
}

void statShow(){ // Parsing dei dati statistiche da un eventuale file sul pc. Il file è automaticamente generato a inizio partita se non esistente.
    char buf[STANDARD_STRLEN];
    int statsArr[CARD_TYPES][STANZE_N], i;
    FILE* stats = fopen(STAT_DEFAULT, "rb");
    if(!stats){ // Se non è mai stata neanche cominciata una partita, o per un errore momentaneo di accesso.
        fprintf(stderr, "Nessun file di statistiche rilevato. Hai giocato ad almeno una partita?\n");
        return;
    }
    else{
        fread(statsArr, CARD_TYPES, STANZE_N*sizeof(int), stats);
        printf("\nStatistiche del crimine:\n\n");
        printf("STANZE\n");
        for(i=0;i<STANZE_N; i++){
            printf("%-16s - %d volte\n", stanze(i, buf), statsArr[STANZA][i]);
        }
        printf("\nARMI\n");
        for(i=0;i<ARMI_N; i++){
            printf("%-16s - %d volte\n", armi(i, buf), statsArr[ARMA][i]);
        }
        printf("\nSOSPETTI\n");
        for(i=0;i<SOSPETTI_N; i++){
            printf("%-16s - %d volte\n", sospetti(i, buf), statsArr[SOSPETTO][i]);
        }
        printf("\n");
        fclose(stats);
    }
}

int checkCard_Archive(char archive[CARD_TYPES][STANZE_N][STANDARD_STRLEN], Carta* card, int len){ //riporta l'indice dell'elemento nel vettore.
    int i;
    for(i=0; i<len; i++) {
        if (strcmp(archive[card->tipo][i], card->desc) == 0) {
            return i;
        }
    }
    return -1;
}

void parseTac(Taccuino* tac){ // Stampa le informazioni contenute nel taccuino caricato da binario.
    Carta* carta;
    int carte, i;
    char buf[STANDARD_STRLEN];
    /*
     * Il formato del taccuino è blocchi di 4 carte per ipotesi sbagliata + carta mostrata,
     * più un eventuale blocco di 3 carte finale per l'ipotesi corretta.
     */
    carta = tac->cima;
    for(carte=0; carte < tac->numCarte/4; carte++) {
        printf("\nHai compiuto questa ipotesi.\n");
        for (i = 0; i < CARD_TYPES; i++) {
            if(!carta){
                fprintf(stderr, "Errore nella lettura di un taccuino esistente e formattato.\n");
                exit(-1);
            }
            printf("\t%-8s : %s\n", tipi(carta->tipo, buf), carta->desc);
            carta = carta->next;
        }
        if (carta) {
            printf("L'ipotesi era errata. La carta mostrata era:\n");
            printf("\t%-8s : %s\n", tipi(carta->tipo, buf), carta->desc);
            carta = carta->next;
        } else {
            printf("L'ipotesi era esatta.\n");
        }
        printf("\n");
    }
}