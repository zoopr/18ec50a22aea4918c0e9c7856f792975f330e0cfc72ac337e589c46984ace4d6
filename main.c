//
// Created by Mat on 18/01/30.
//
// Interfaccia di inizializzazione e setup del tavolo da gioco.
// Selettore AI mode e visualizzatore statistiche.
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
    char buffer[SBUF];
    _Bool next_phase, AI_mode = false;
    Tabellone* tavolo;

    srand((unsigned int)time(NULL)); //inizializziamo il seed per tutte le operazioni di partita.



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
                           "3 - Statistiche\n");
            scanf("%s", buffer);
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
                default:
                    printf("Opzione non disponibile.\n\n");
            }
        } while (!next_phase);

        next_phase = false;
        do { //secondo menu. Inizializzazione tavolo da gioco.
            printf("Selezionare un'opzione.\n"
                           "1 - Comincia partita\n"
                           "2 - Carica partita\n");
            scanf("%s", buffer);
            option = strtol(buffer,NULL, 10);
            switch (option) {
                case 1:
                    tavolo = FreshStart(AI_mode);
                    next_phase = true;
                    break;
                case 2:
                    printf("Inserire il nome del file di salvataggio da caricare\n");
                    scanf("%s", buffer);
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
        //Alla fine di una partita.
        printf("\nProcedere con un'altra partita? S/N\n");
        scanf("%s", buffer);
        switch (tolower(buffer[0])) {
            case 's':
                next_phase = true;
                break;
            default:
                next_phase = false;
        }
    }while(next_phase);
    printf("\nArrivederci!\n");
    return 0;
}