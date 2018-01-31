//
// Created by Mat on 18/01/30.
//
// Contiene Tutti gli standard e le costanti al quale devono poter accedere i diversi elementi del gioco
// a vari punti della partita.
//

#ifndef CLUEDO_STANDARDS_H
#define CLUEDO_STANDARDS_H

#define STANDARD_STRLEN 24
#define STANZE_N 9
#define ARMI_N 6
#define SOSPETTI_N 6
#define GIOCATORI_MAX 6
#define GIOCATORI_MIN 3
#define SBUF 256
#define LOG_DEFAULT "log.txt"

typedef enum {ARMA, SOSPETTO, STANZA} tipoCarta;

char* tipi(int index, char* copyArea);
char* stanze(int index, char* copyArea);
char* armi(int index, char* copyArea);
char* sospetti(int index, char* copyArea);
void copiaMoveset(int copyArea[STANZE_N][STANZE_N]);
char* dtoc(int decimal, char copyArea[STANDARD_STRLEN]);

#endif //CLUEDO_STANDARDS_H
