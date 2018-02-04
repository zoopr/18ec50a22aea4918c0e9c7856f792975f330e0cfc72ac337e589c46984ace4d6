//
// Created by Mat on 18/01/30.
//

#include <string.h>
#include <stdio.h>
#include "Standards.h"

char* tipi(int index, char* copyArea) {
    char tipi[CARD_TYPES][STANDARD_STRLEN] = {"Arma", "Sospetto", "Stanza"};
    if(index <CARD_TYPES && index>-1) { // assicuriamoci che non stiamo chiamando parti di memoria a caso per errore
        strncpy(copyArea, tipi[index], STANDARD_STRLEN);
        return copyArea;
    }else{
        printf("Errore nell'offset di ingresso!\n");
        return NULL;
    }
}

char* stanze(int index, char* copyArea) {
    char stanze[STANZE_N][STANDARD_STRLEN] = {"Laboratorio T", "Laboratorio M", "Bagno", "Aula Costa",
                                                           "Giardino", "Entrata", "Parcheggio", "Simaz", "Batcaverna"};
    if(index <STANZE_N && index>-1) {
        strncpy(copyArea,stanze[index], STANDARD_STRLEN);
        return copyArea;
    }else{
        printf("Errore nell'offset di ingresso!\n");
        return NULL;
    }
}
char* armi(int index, char* copyArea){
    char armi [ARMI_N][STANDARD_STRLEN] = {"Cavo di rete", "Sparacoriandoli", "Calcinaccio", "Tastiera USB",
                                                    "Fumo caldaia", "Floppy disk 3.5"};
    if(index <ARMI_N && index>-1) {
        strncpy(copyArea, armi[index], STANDARD_STRLEN);
        return copyArea;
    }else{
        printf("Errore nell'offset di ingresso!\n");
        return NULL;
    }
}

char* sospetti(int index, char* copyArea) {
    char sospetti[SOSPETTI_N][STANDARD_STRLEN] = {"Alan Turing", "G. M. Hopper", "Edsger Dijkstra", "G. F. Voronoj",
                                                           "J. V. Neumann", "M. Hamilton"};
    if(index <SOSPETTI_N && index>-1) {
        strncpy(copyArea, sospetti[index], STANDARD_STRLEN);
        return copyArea;
    }else{
        printf("Errore nell'offset di ingresso!\n");
        return NULL;
    }
}

void copiaMoveset(int copyArea[STANZE_N][STANZE_N]) {
    int i, j;
    int moveset[STANZE_N][STANZE_N] = {
            0, 4, 8, 4, 8, 12, 16, 12, 14,
            4, 0, 4, 8, 12, 8, 12, 8, 10,
            8, 4, 0, 12, 16, 12, 8, 4, 6,
            4, 8, 12, 0, 4, 8, 12, 8, 10,
            8, 12, 16, 4, 0, 4, 8, 12, 10,
            12, 8, 12, 8, 4, 0, 4, 8, 6,
            16, 12, 8, 12, 8, 4, 0, 4, 2,
            12, 8, 4, 8, 12, 8, 4, 0, 2,
            14, 10, 6, 10, 10, 6, 2, 2, 0};
    for (i=0; i<STANZE_N; i++){
        for(j=0; j<STANZE_N; j++){
            copyArea[i][j] = moveset[i][j];
        }
    }
}

char* dtoc(int decimal, char copyArea[STANDARD_STRLEN]){ //Da decimale a stringa. Supporto fino a 10^23, ben sopra il limite di un int da 4 byte. Usato nei dadi e nel turno.
    int i, j;
    char buf[STANDARD_STRLEN];

    for(i =0; i<STANDARD_STRLEN-1 && decimal>0; i++, decimal/=10){
        buf[i] = (char)('0' + decimal%10);
    }
    buf[i] = '\0'; //per usare correttamente strlen;
    for(j=0, i--; j<strlen(buf); i--, j++){ //deincremento i per evitare di scrivere null nel primo carattere.
        copyArea[j] = buf[i];
    }
    copyArea[j] = '\0';

    return copyArea;
}


