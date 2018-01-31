#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "BoardManager.h"
#include "Gameplay.h"

int main() {

    int option;
    char buffer[SBUF];
    _Bool next_phase, AI_mode = false;
    Tabellone* tavolo;

    srand(time(NULL)); //inizializziamo il seed per tutte le operazioni di partita.



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
    next_phase = false;

    do{ //primo menu.
        printf("Selezionare un'opzione.\n"
                       "1 - Comincia partita\n"
                       "2 - Carica partita\n"
                       "3 - Statistiche\n");

        scanf("%d", &option);
        switch(option){
            case 1:
                tavolo = FreshStart();
                next_phase = true;
                break;
            case 2:
                printf("Inserire il nome del file di salvataggio da caricare\n");
                scanf("%s", buffer);
                tavolo = LoadBoard(buffer);
                if (tavolo)
                    next_phase = true;
                break;
            case 3:
                statShow();
                break;
            default:
                printf("Opzione non disponibile.\n\n");
        }
    }while(!next_phase);

    next_phase = false;
    do{ //secondo menu. AI mode
        printf("Modalità di gioco\n"
                       "1 - Classica\n"
                       "2 - AI mode\n");
        scanf("%d", &option);
        switch(option){
            case 1:
                AI_mode = false;
                next_phase = true;
                break;
            case 2:
                AI_mode = true;
                next_phase = true;
                break;
                //printf("Modalità non disponibile.\n\n");
                //break;
            default:
                printf("Opzione non disponibile.\n\n");
        }
    }while(!next_phase);

    if (AI_mode){
        //TODO
        MainGame(tavolo, Turn_AI);
    }
    else{
        MainGame(tavolo, Turn);
    }

    return 0;
}