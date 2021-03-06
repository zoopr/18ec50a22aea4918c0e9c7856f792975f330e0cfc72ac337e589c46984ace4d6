//
// Created by Mat on 18/01/30.
//
// Funzioni base, intermedie e avanzate del gioco.
// Filename completamente personalizzabile e caricabile, taccuino contenente ipotesi passate e carte mostrateci, e logger.
// Supporto per il caricamento di salvataggi con nomi sospetti e carte custom (non estendibile alle stanze a causa della
// necessaria ed esclusivamente implicita corrispondenza stringa-id numerica delle specifiche).
// AI di gioco basata su scelta casuale pesata da pseudo-training con supervisione dei turni precedenti.
// Gestisce la scelta dell'ipotesi (incluso il movimento in condizioni non ottimali) e quali carte mostrare.
// Reagisce a ipotesi sbagliate e corrette.
// Modulo di statistiche permanenti richiamabile dal menu principale e mostrato all'inizio della modalità AI (Supporta
// salvataggi di terze parti ma non stringhe custom).
//
// Il modulo main contiene il menu principale e il livello più alto della gestione delle opzioni di partenza del gameplay.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "BoardManager.h"
#include "Gameplay.h"

int main() {
    int option;
    char buffer[SBUF], *pos;
    _Bool next_phase, AI_mode, exit_flag = false;
    Tabellone* tavolo;

    srand((unsigned int)time(NULL)); //inizializziamo il seed per tutte le operazioni di partita.
    logger("\nProgramma avviato.\n");

    printf("\n"
           " .d8888b.  888      88888     888 8888888888 8888888b.   .d88888b.  \n"
           "d88P  Y88b 888      88888     888 888        888  \"Y88b d88P\" \"Y88b \n"
           "888    888 888        888     888 888        888    888 888     888 \n"
           "888        888        888     888 8888888    888    888 888     888 \n"
           "888        888        888     888 888        888    888 888     888 \n"
           "888    888 888        888     888 888        888    888 888     888 \n"
           "Y88b  d88P 88888888   Y88b. .d88P 8888888888 888  .d88P Y88b. .d88P \n"
           " \"Y8888P\"  88888888    \"Y88888P\"  8888888888 8888888P\"   \"Y88888P\"  \n"
           " __                                          \n"
           "|  \\ .  _   _   _  .    _  _| . |_ .  _   _  \n"
           "|__/ | (_| | ) | ) |   (- (_| | |_ | (_) | ) \n"
           "     /                                                                  \n"
           "                                                                  \n"
                   );
    do {
        next_phase = false;
        do { //primo menu. Selezione umana o AI.
            printf("Modalità di gioco\n"
                           "1 - Classica\n"
                           "2 - AI mode\n"
                           "3 - Statistiche\n"
                           "4 - Esci\n");
            inputWrapper(buffer, &pos);

            option = strtol(buffer,NULL, 10);
            switch (option) {
                case 1:
                    AI_mode = false;
                    next_phase = true;
                    break;
                case 2:
                    AI_mode = true;
                    next_phase = true;
                    break;
                case 3:
                    statShow();
                    break;
                case 4:
                    next_phase = true;
                    exit_flag = true;
                    break;
                default:
                    printf("Opzione non disponibile.\n\n");
            }
        } while (!next_phase);

        //Usciamo dal programma se riceviamo il prompt adeguato.
        if(exit_flag){
            continue;
        }
        next_phase = false;
        do { //secondo menu. Inizializzazione tavolo da gioco.
            printf("Selezionare un'opzione.\n"
                           "1 - Comincia partita\n"
                           "2 - Carica partita\n");
            inputWrapper(buffer, &pos);

            option = strtol(buffer,NULL, 10);
            switch (option) {
                case 1:
                    tavolo = FreshStart(AI_mode);
                    if(tavolo)
                        next_phase = true;
                    break;
                case 2:
                    printf("Inserire il nome del file di salvataggio da caricare\n");
                    inputWrapper(buffer, &pos);
                    tavolo = LoadBoard(buffer);
                    if (tavolo)
                        next_phase = true;
                    break;
                default:
                    printf("Opzione non disponibile.\n\n");
            }
        } while (!next_phase);

        if (AI_mode) {
            MainGame(tavolo, Turn_AI, AI_mode);
        } else {
            MainGame(tavolo, Turn, AI_mode);
        }
        //Alla fine di una partita, libera la memoria allocata dinamicamente e invia il prompt per rigiocare.
        FreeBoard(tavolo);

        next_phase = false;
        printf("\nProcedere con un'altra partita? S/N\n");
        do{
            inputWrapper(buffer, &pos);
            switch (tolower(buffer[0])) {
                case 's':
                    exit_flag = false;
                    next_phase = true;
                    break;
                case 'n':
                    exit_flag = true;
                    next_phase = true;
                    break;
                default:
                    printf("Opzione non riconisciuta. Inserire risposta adeguata.\n");
                    break;
            }
        }while(!next_phase);
    }while(!exit_flag);
    printf("\nArrivederci!\n");
    logger("Programma terminato.\n");
    return 0;
}