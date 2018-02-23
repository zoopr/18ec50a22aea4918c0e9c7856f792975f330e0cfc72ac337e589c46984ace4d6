//
// Created by Mat on 18/01/30.
//
// Contiene tutti gli standard e le costanti di rilevanza globale nel progetto,
// incluse qualsiasi funzione di utility non disponibile nelle librerie standard.
// Le funzioni sono commentate lungo il sorgente.
//

#ifndef CLUEDO_STANDARDS_H
#define CLUEDO_STANDARDS_H

#define STANDARD_STRLEN 24 //lunghezza delle stringhe fondamentali delle strutture.
#define CARD_TYPES 3
#define STANZE_N 9
#define ARMI_N 6
#define SOSPETTI_N 6
#define GIOCATORI_MAX 6
#define GIOCATORI_MIN 3
#define SBUF 256 //string buffer usato per ricevere linee stringa e altri dati dall'utente.
#define LOG_DEFAULT "log.txt"
#define STAT_DEFAULT "stats.dat"

typedef enum {ARMA, SOSPETTO, STANZA} tipoCarta;

char* tipi(int index, char* copyArea);
char* stanze(int index, char* copyArea);
char* armi(int index, char* copyArea);
char* sospetti(int index, char* copyArea);
void copiaMoveset(int copyArea[STANZE_N][STANZE_N]);
char* dtoc(int decimal, char copyArea[STANDARD_STRLEN]);
void inputWrapper(char buf[SBUF], char** p);

#endif //CLUEDO_STANDARDS_H
