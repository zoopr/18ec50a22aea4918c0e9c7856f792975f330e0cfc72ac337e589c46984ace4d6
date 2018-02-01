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
    int numGiocatori, cartePerGiocatore, i, j;
    Mazzo *mainDeck, *second, *third;
    Carta* carta;
    FILE* tac;
    char buffer[STANDARD_STRLEN + 4];

    Tabellone* tavolo = (Tabellone*)malloc(sizeof(Tabellone));
    if(!tavolo){
        exit(-1);
    }
    copiaMoveset(tavolo->layout); //copia il layout.
    tavolo->giocatori = playerInit(&numGiocatori); //salviamo il numero all'interno di numGiocatori per le funzioni successive.
    tavolo->numeroTurni = 0;
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

    mainDeck->numCarte--;
    second->numCarte--;
    third->numCarte--;
    tavolo->soluzione.numCarte = CARD_TYPES;
    printf("Omicidio eseguito. RIP Djanni.\n");


    mainDeck = mergeDecks(mainDeck, mergeDecks(second, third)); //uniamo i tre mazzi in fila
    second = third = NULL; //per sicurezza visto che abbiamo liberato i mazzi contenitore in mergeDecks().
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

    for (i=0; i<numGiocatori; i++){ //inizializzazione taccuini
        strcpy(buffer, tavolo->giocatori[i].nome);
        strcat(buffer, ".tac");
        tac = fopen(buffer, "w"); //la tag w ci assicura che se esiste viene azzerato.
        if(!tac)
            exit(-2);
        fprintf(tac, "Le carte sul tavolo sono %d\n", tavolo->carteScoperte.numCarte);
        carta = tavolo->carteScoperte.cima;
        for (j=0; j<tavolo->carteScoperte.numCarte; j++){
            fprintf(tac, "%s : %s\n", tipi(carta->tipo, buffer), carta->desc); //riutilizziamo buffer una volta aperto il file.
            if (carta->next)
                carta = carta->next;
        }
        fprintf(tac, "Le carte nella tua mano sono %d\n", tavolo->giocatori[i].mano.numCarte );
        carta = tavolo->giocatori[i].mano.cima;
        for (j=0; j<tavolo->giocatori[i].mano.numCarte; j++){
            fprintf(tac, "%s : %s\n", tipi(carta->tipo, buffer), carta->desc);
            if (carta->next)
                carta = carta->next;
        }
        fprintf(tac, "Le carte scoperte degli avversari sono:\n"); //per ora rimane vuoto. si popolerà nella ricerca dopo le ipotesi.

        fclose(tac); // possiamo usare a+ ai turni successivi.
    }

    statInit(tavolo);

    logger("\nCominciata nuova partita.\n");

    return tavolo;

}

Tabellone* LoadBoard(char* filename){
    int i, j;
    Giocatore *ultimoG;
    Carta *ultimaC, *codaC;
    char msgbuf[SBUF] = "\nCaricata partita da ";

    FILE* save = fopen(filename, "r");
    if (!save){
        printf("Errore nel caricamento del file\n"
        "Verificare il nome e la posizione del file di salvataggio.\n");
        return NULL;
    }
    else {
        Tabellone *table = (Tabellone *) malloc(sizeof(Tabellone)); //seguendo il formato byte a byte delle specifiche.
        if (!table)
            exit(-1);
        fread(table, 3, sizeof(int), save); //Scrive i primi 3 int da disco, incluso il numero di giocatori necessario dopo.
        table->giocatori = (Giocatore*) calloc(table->numGiocatori, sizeof(Giocatore)); //Allochiamo un vettore di dimensione variabile.
        if (!table->giocatori)                                                          //in c89 non si può fare in allocazione automatica.
            exit(-1);
        for (i = 0; i < table->numGiocatori; i++) {
            ultimoG = &table->giocatori[i];
            fread(ultimoG, 1, STANDARD_STRLEN * sizeof(char) + 2 * sizeof(int), save);
            fread(&ultimoG->mano.numCarte, 1, sizeof(int), save); //Simile ai giocatori ma ogni carta è un nodo di LL
            for (j = 0; j < ultimoG->mano.numCarte; j++) {        //e come tale lo possiamo allocare individualmente
                ultimaC = (Carta *) malloc(sizeof(Carta));        //avendo il riferimento all'elemento successivo esplicito.
                if (!ultimaC)
                    exit(-1);
                fread(&ultimaC->tipo, 1, sizeof(tipoCarta) + STANDARD_STRLEN*sizeof(char), save); //Il salvataggio contiene l'ordine e i valori non di link.
                if (!ultimoG->mano.cima) {                                                        //Costruiamo i link della lista.
                    ultimoG->mano.cima = ultimaC;
                    ultimaC->next = NULL;
                } else {                                                                          //Aggiungiamo in coda dalla second carta in poi.
                    codaC = ultimoG->mano.cima;                                                   //Preserviamo l'ordine con cui erano elencate.
                    while (codaC->next)
                        codaC = codaC->next;
                    codaC->next = ultimaC;
                    ultimaC->next = NULL;
                }

            }

        }
        fread(&table->carteScoperte.numCarte, 1, sizeof(int), save); //Carichiamo le carte scoperte.
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
        table->soluzione.numCarte = CARD_TYPES; //le carte segrete sono hardcoded per essere 3(o perlomeno una per tipo).
        table->soluzione.cima = NULL;
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
                ultimaC->next = NULL;
            }

        }
        fclose(save);
        //non ci preoccupiamo dei taccuini. La tag "a" crea i file se non esistenti.
        //...anche se taccuini di terze parti formattati in altri modi possono essere disorientanti.

        statInit(table); // Carichiamo le statistiche da un secondo file.

        strcat(msgbuf, filename);
        strcat(msgbuf, "\n");
        logger(msgbuf);


        return table;
    }

}

void MainGame(Tabellone* tavolo, _Bool(*turnType)(Tabellone*, Giocatore*), _Bool AI){
    _Bool winner = 0;
    int i;
    char buf[SBUF];
    float loadArea[CARD_TYPES][STANZE_N];

    if(AI){
        for (i=0; i<tavolo->numGiocatori; i++){
            tavolo->turnoCorrente = (tavolo->turnoCorrente+1)%tavolo->numGiocatori;
            initInterest(tavolo, loadArea); //
            showingStrategy(tavolo, &tavolo->giocatori[i], NULL, 0); // Inizializziamo le carte da nascondere a prescindere.
        }
    }


    while(!winner){
        tavolo->turnoCorrente = (tavolo->turnoCorrente+1)%tavolo->numGiocatori;
        tavolo->numeroTurni +=1;

        printf("Vuoi salvare lo stato attuale della partita? S/N\n");
        scanf("%23s", buf);
        if(tolower(buf[0]) == 's'){
            printf("Inserire il nome del file in cui salvare.(max %d caratteri)\n", STANDARD_STRLEN-1);
            scanf("%23s", buf);
            saveState(buf, tavolo);
        }
        winner = turnType(tavolo, &tavolo->giocatori[tavolo->turnoCorrente]);
    }
    strcpy(buf, tavolo->giocatori[tavolo->turnoCorrente].nome);
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
           "...almeno fino al gioco dell'anno prossimo.\n", tavolo->giocatori[tavolo->turnoCorrente].nome);

    statTrack(tavolo);
    statSave(tavolo);

    logger("Partita finita.\n");
    printf("Premere un pulsante per terminare\n");
    while(!getchar());
}

_Bool Turn(Tabellone* tavolo, Giocatore* giocatore){
    int dice[2];
    _Bool reachable[STANZE_N];
    char buf[CARD_TYPES][STANDARD_STRLEN] ;  //per lo storage temporaneo delle stringhe da stampare e della ipotesi.

    strcpy(buf[0], "\nTURNO ");
    strcat(buf[0],  dtoc(tavolo->numeroTurni, buf[1]));
    strcat(buf[0], " - Giocatore: ");
    strcat(buf[0], giocatore->nome);
    strcat(buf[0], "\n");
    printf(buf[0]);
    logger(buf[0]);

    leggiTaccuino(giocatore->nome);
    if (giocatore->ipotesiEsatta){
        printf("Hai già compiuto l'ipotesi esatta.\n"
               "Procedi direttamente al lancio dei dadi.\n");
        strcpy(buf[0], "IL giocatore sta cercando di fare dadi doppi.\n");
        logger(buf[0]);
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
        printf("Posizione attuale: %s\n", stanze(giocatore->stanza, buf[0]));
        printf("Vuoi procedere direttamente con l'ipotesi o lanciare i dadi?\n");
        do{
            printf("1 - Lancio dadi\n"
                           "2 - Ipotesi diretta\n ");
            scanf("%d", &dice[0]); //La variabile non è stata ancora usata in questo scopo.
        }while(dice[0] !=1 && dice[0] !=2);
        if (dice[0] == 1){
            rollDice(dice);
            dice[0] = dice[0] + dice[1]; //non abbiamo bisogno di conservare il valore dei singoli dadi.
            validPaths(tavolo->layout[giocatore->stanza], dice[0], reachable);
            printf("Movimenti validi:\n");
            for(dice[0] = 0, dice[1] = 0; dice[0] < STANZE_N; dice[0]++){
                if(reachable[dice[0]]){
                    printf("%d - %s\n", dice[0], stanze(dice[0], buf[0]));
                    dice[1]++;
                }
            }
            //Decisione spostamento se disponibile
            if(dice[1] > 1){
                printf("\nIn quale stanza desideri muoverti?\n");
                scanf("%d", &dice[1]);
                while(dice[1] >= STANZE_N || dice[1] < 0 ||!reachable[dice[1]]){ //short circuit ci permette di mettere la terza cond.
                    printf("Posizione non raggiungibile. Inserire stanza ammessa.\n");
                    scanf("%d", &dice[1]);
                }
                giocatore->stanza = dice[1];
                strcpy(buf[0], "Giocatore spostato in ");
                strcat (buf[0], stanze(dice[1],buf[1]));
                strcat(buf[0], "\n");
                printf(buf[0]);
                logger(buf[0]);

            }else {
                printf("Giocatore obbligato a rimanere in %s\n", stanze(giocatore->stanza, buf[0]));
            }
        }
        //Formulazione ipotesi
        printf("L'ipotesi è obbligata a svolgersi nella tua stanza attuale(%s).\n", stanze(giocatore->stanza, buf[0]));
        printf("Con quale arma si è compiuto il delitto?\n");
        for(dice[0] = 0; dice[0]<ARMI_N; dice[0]++){
            printf("%d - %s\n", dice[0], armi(dice[0], buf[0]));
        }
        scanf("%d", &dice[0]);
        while(dice[0] >= ARMI_N || dice[0] < 0 ){
            printf("Valore non ammesso. Inserire un numero adeguato.\n");
            scanf("%d", &dice[0]);
        }

        printf("Quale sospetto ha compiuto il delitto?\n");
        for(dice[1] = 0; dice[1]<ARMI_N; dice[1]++){
            printf("%d - %s\n", dice[1], sospetti(dice[1], buf[0]));
        }
        scanf("%d", &dice[1]);
        while(dice[1] >= SOSPETTI_N || dice[1] < 0 ){
            printf("Valore non ammesso. Inserire un numero adeguato.\n");
            scanf("%d", &dice[1]);
        }
        giocatore->ipotesiEsatta = checkSolution(stanze(giocatore->stanza, buf[0]), armi(dice[0], buf[1]), sospetti(dice[1], buf[2]), tavolo, 0, NULL);
        if(giocatore->ipotesiEsatta){
            printf("Ipotesi esatta!\n"
                           "Per vincere, ottieni dadi doppi in uno dei prossimi turni.\n");
        }
        printf("Turno finito.\n\n");



    }
}

_Bool Turn_AI(Tabellone* tavolo, Giocatore* giocatore){ //Control flow più aderente possibile alla partita tradizionale, ma con feature che è meglio separare accuratamente.
    int dice[D_N];
    _Bool reachable[STANZE_N];
    char buf[CARD_TYPES][STANDARD_STRLEN] ;  //per lo storage temporaneo delle stringhe da stampare e della ipotesi.
    float interest[CARD_TYPES][STANZE_N]; //parte della logica decisionale AI

    strcpy(buf[0], "\nTURNO ");
    strcat(buf[0],  dtoc(tavolo->numeroTurni, buf[1]));
    strcat(buf[0], " - Giocatore: ");
    strcat(buf[0], giocatore->nome);
    strcat(buf[0], "\n");
    printf(buf[0]);
    logger(buf[0]);

    printTableStatus(tavolo); //stampa la posizione di ogni carta distribuita.
    readInterest(tavolo, interest); // Carica la matrice esistente nell'interesse del giocatore corrente.

    if (giocatore->ipotesiEsatta){ //Non c'è interazione in questa branch. Rimane identica alla versione umana.
        printf("Hai già compiuto l'ipotesi esatta.\n"
                       "Procedi direttamente al lancio dei dadi.\n");
        strcpy(buf[0], "Il giocatore sta cercando di fare dadi doppi.\n");
        logger(buf[0]);
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
        printf("Posizione attuale: %s\n", stanze(giocatore->stanza, buf[0]));
        printf("Procedere al lancio dei dadi.\n");
        //La scelta ottimale per l'AI (ma anche per i giocatori umani) è sempre di valutare le opzioni di spostamento ed eventualmente stare fermi.
        rollDice(dice);
        dice[0] = dice[0] + dice[1]; //non abbiamo bisogno di conservare il valore dei singoli dadi.
        validPaths(tavolo->layout[giocatore->stanza], dice[0], reachable);
        printf("Movimenti validi:\n");
        for(dice[0] = 0, dice[1] = 0; dice[0] < STANZE_N; dice[0]++){
            if(reachable[dice[0]]){
                printf("%d - %s\n", dice[0], stanze(dice[0], buf[0]));
                dice[1]++;
            }
        }
        //Decisione spostamento se disponibile
        if(dice[1] > 1){
            printf("\nIn quale stanza desideri muoverti?\n");
            dice[1] = movementStrategy(interest[0], reachable);
            printf("L'AI ha scelto l'opzione %d\n", dice[1]);
            while(dice[1] >= STANZE_N || dice[1] < 0 ||!reachable[dice[1]]){ //short circuit ci permette di mettere la terza cond.
                printf("Posizione non raggiungibile. Inserire stanza ammessa.\n");
                scanf("%d", &dice[1]); //L'AI non sbaglia, specie con l'accesso diretto alla mask reachable. se atterriamo qua c'è da fare debugging.
            }
            giocatore->stanza = dice[1];
            strcpy(buf[0], "Giocatore spostato in ");
            strcat (buf[0], stanze(dice[1],buf[1]));
            strcat(buf[0], "\n");
            printf(buf[0]);
            logger(buf[0]);

        }else {
            printf("Giocatore obbligato a rimanere in %s\n", stanze(giocatore->stanza, buf[0]));
        }

        //Formulazione ipotesi
        printf("L'ipotesi è obbligata a svolgersi nella tua stanza attuale(%s).\n", stanze(giocatore->stanza, buf[0]));
        printf("Con quale arma si è compiuto il delitto?\n");
        for(dice[0] = 0; dice[0]<ARMI_N; dice[0]++){
            printf("%d - %s\n", dice[0], armi(dice[0], buf[0]));
        }
        dice[0] = suspectStrategy(interest[1]);
        printf("L'AI ha scelto l'opzione %d\n", dice[0]);
        while(dice[0] >= ARMI_N || dice[0] < 0 ){ //come nelle stanze, questa opzione è qua per il debugging e per il testing di feature diverse.
            printf("%d: Valore non ammesso. Inserire un numero adeguato.\n", dice[0]);
            scanf("%d", &dice[0]);
        }

        printf("Quale sospetto ha compiuto il delitto?\n");
        for(dice[1] = 0; dice[1]<ARMI_N; dice[1]++){
            printf("%d - %s\n", dice[1], sospetti(dice[1], buf[0]));
        }
        dice[1] = suspectStrategy(interest[2]);
        printf("L'AI ha scelto l'opzione %d\n", dice[1]);
        while(dice[1] >= SOSPETTI_N || dice[1] < 0 ){
            printf("%d: Valore non ammesso. Inserire un numero adeguato.\n", dice[1]);
            scanf("%d", &dice[1]);
        }
        giocatore->ipotesiEsatta = checkSolution(stanze(giocatore->stanza, buf[0]), armi(dice[0], buf[1]), sospetti(dice[1], buf[2]), tavolo, 1, interest);
        if(giocatore->ipotesiEsatta){
            printf("Ipotesi esatta!\n"
                           "Per vincere, ottieni dadi doppi in uno dei prossimi turni.\n\n");
        }else{
            /*
             * C'è una possibilità statistica non indifferente che le carte parte di una soluzione sbagliata ma non
             * trovate nella mano di nessuno possano non essere state chiamate perchè tra le carte segrete.
             * Questa sezione aggiusta un bias per le carte chiamate più volte e mai contestate.
             * La carta trovata è stata in precedenza azzerata.
             */
            interest[0][giocatore->stanza]*=INTEREST_FACTOR;
            interest[1][dice[0]]*=INTEREST_FACTOR;
            interest[2][dice[1]]*=INTEREST_FACTOR;
        }
        printf("Turno finito.\n\n");
        saveInterest(tavolo, interest);
    }
}