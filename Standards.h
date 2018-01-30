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

typedef enum {ARMA, SOSPETTO, STANZA} tipoCarta;
extern const char stanze[STANZE_N][STANDARD_STRLEN];
extern const char armi [ARMI_N][STANDARD_STRLEN];
extern const char sospetti[SOSPETTI_N][STANDARD_STRLEN];

extern const int moveset[STANZE_N][STANZE_N];


#endif //CLUEDO_STANDARDS_H
