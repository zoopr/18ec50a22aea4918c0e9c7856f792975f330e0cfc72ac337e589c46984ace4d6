//
// Created by Mat on 18/01/30.
//

#include "Standards.h"

const char stanze[STANZE_N][STANDARD_STRLEN] = {"Laboratorio T", "Laboratorio M", "Bagno", "Aula Costa", "Giardino",
                                                "Entrata", "Parcheggio", "Simaz", "Batcaverna"};
const char armi [ARMI_N][STANDARD_STRLEN] = {"Cavo di rete", "Sparacoriandoli", "Calcinaccio", "Tastiera USB",
                                             "Fumo caldaia", "Floppy disk 3.5"};
const char sospetti[SOSPETTI_N][STANDARD_STRLEN] = {"Alan Turing", "G. M. Hopper", "Edsger Dijkstra", "G. F. Voronoj",
                                                    "J. V. Neumann", "M. Hamilton"};

const int moveset[STANZE_N][STANZE_N] = {
        0,  4,  8,  4,  8, 12, 16, 12, 14,
        4,  0,  4,  8, 12,  8, 12,  8, 10,
        8,  4,  0, 12, 16, 12,  8,  4,  6,
        4,  8, 12,  0,  4,  8, 12,  8, 10,
        8, 12, 16,  4,  0,  4,  8, 12, 10,
        12,  8, 12,  8,  4,  0,  4,  8,  6,
        16, 12,  8, 12,  8,  4,  0,  4,  2,
        12,  8,  4,  8, 12,  8,  4,  0,  2,
        14, 10,  6, 10, 10,  6,  2,  2,  0};