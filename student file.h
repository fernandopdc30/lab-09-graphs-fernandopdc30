#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARRETERAS 50
#define MAX_ELEMENTO_CARRETERA 50

typedef struct {
    char id[MAX_ELEMENTO_CARRETERA];
    char ciudad1[MAX_ELEMENTO_CARRETERA];
    char ciudad2[MAX_ELEMENTO_CARRETERA];
    int costo;
} Carretera;

typedef struct {
    char padre[MAX_ELEMENTO_CARRETERA];
    int costoTotal;
} Ciudad;

void inicializarCiudades(Ciudad ciudades[], int numCiudades) {
    for (int i = 0; i < numCiudades; i++) {
        strcpy(ciudades[i].padre, "");
        ciudades[i].costoTotal = 0;
    }
}

int encontrarPadre(Ciudad ciudades[], int ciudad) {
    if (strcmp(ciudades[ciudad].padre, "") == 0) {
        return ciudad;
    } else {
        return encontrarPadre(ciudades, atoi(ciudades[ciudad].padre));
    }
}

void unirCiudades(Ciudad ciudades[], int ciudad1, int ciudad2) {
    int padreCiudad1 = encontrarPadre(ciudades, ciudad1);
    int padreCiudad2 = encontrarPadre(ciudades, ciudad2);

    if (padreCiudad1 != padreCiudad2) {
        sprintf(ciudades[padreCiudad1].padre, "%d", padreCiudad2);
    }
}

int compararCarreteras(const void *a, const void *b) {
    return strcmp(((Carretera *)a)->id, ((Carretera *)b)->id);
}

char* reconstruye(char* carreteras[], int numCarreteras) {
    Carretera arregloCarreteras[MAX_CARRETERAS];
    Ciudad ciudades[MAX_CARRETERAS];

    for (int i = 0; i < MAX_CARRETERAS; i++) {
        strcpy(arregloCarreteras[i].id, "");
        strcpy(arregloCarreteras[i].ciudad1, "");
        strcpy(arregloCarreteras[i].ciudad2, "");
        arregloCarreteras[i].costo = 0;

        strcpy(ciudades[i].padre, "");
        ciudades[i].costoTotal = 0;
    }

    for (int i = 0; i < numCarreteras; i++) {
        sscanf(carreteras[i], "%s %s %s %d", arregloCarreteras[i].id, arregloCarreteras[i].ciudad1,
               arregloCarreteras[i].ciudad2, &arregloCarreteras[i].costo);

        if (arregloCarreteras[i].costo == 0) {
            ciudades[atoi(arregloCarreteras[i].ciudad1)].costoTotal += 1;
            ciudades[atoi(arregloCarreteras[i].ciudad2)].costoTotal += 1;
        }
    }

    qsort(arregloCarreteras, numCarreteras, sizeof(Carretera), compararCarreteras);

    inicializarCiudades(ciudades, MAX_CARRETERAS);

    for (int i = 0; i < numCarreteras; i++) {
        if (arregloCarreteras[i].costo > 0) {
            int ciudad1 = atoi(arregloCarreteras[i].ciudad1);
            int ciudad2 = atoi(arregloCarreteras[i].ciudad2);

            unirCiudades(ciudades, ciudad1, ciudad2);
        }
    }

    int numComponentesConexas = 0;
    int componenteConexa = -1;

    for (int i = 0; i < MAX_CARRETERAS; i++) {
        if (ciudades[i].costoTotal > 0) {
            if (strcmp(ciudades[i].padre, "") == 0) {
                numComponentesConexas++;
                componenteConexa = i;
            }
        }
    }

    if (numComponentesConexas > 1) {
        return "IMPOSIBLE";
    }

    char* resultado = (char*)malloc(MAX_CARRETERAS * MAX_ELEMENTO_CARRETERA);
    strcpy(resultado, "");

    for (int i = 0; i < numCarreteras; i++) {
        if (arregloCarreteras[i].costo > 0) {
            strcat(resultado, arregloCarreteras[i].id);
            strcat(resultado, " ");
        }
    }

    return resultado;
}
