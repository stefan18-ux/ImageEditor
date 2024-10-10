#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
#define MAX 100

int main() {
    // initializam un vector ce contine matricile asociate unei imagini incarcate
    int ****matrix_array = NULL;
    int num_matrix = 0;
    // initializam un vector de filtre ce retine matricile ce definesc fiecare filtru
    float ***filter_array = NULL;
    int num_filters = 0;
    /* matrice ce retine dimensiunile matricei aflata la indexul index; linia desemneaza indexul, iar componentele [index][0] si
    [index][1] numarul de linii, respectiv numarul de coloane */
    int **matrix_dim = NULL;
    // vector ce retine dimensiunile filtrelor aflate la indexul index
    int *filter_size = NULL;
    // incepem procesarea interactiva, programul primeste comenzi si le efectueza pana la primirea comenzii de exit
    char command[3];
    while (1) {
        scanf("%s", command);
        // Exit
        if (strcmp(command, "e") == 0) {
            // dezalocam vectorul de matrici ale imaginilor
            for (int i = 0; i < num_matrix; i++) {
                matrix_free(matrix_array[i], matrix_dim[i][0], matrix_dim[i][1]);
            }
            free(matrix_array);
            // dezalocam matricea ce retine dimensiunile imaginilor
            for (int i = 0; i < num_matrix; i++) {
                free(matrix_dim[i]);
            }
            free(matrix_dim);
            // dezalocam vectorul ce retine matricile caracteristice fiecarui filtru
            for (int i = 0; i < num_filters; i++) {
                for (int j = 0; j < filter_size[i]; j++) {
                    free(filter_array[i][j]);
                }
                free(filter_array[i]);
            }
            free(filter_array);
            // dezalocam vectorul ce retine dimensiunile filtrelor
            free(filter_size);
            // se forteaza iesirea din loop
            break;
        }
        // Load
        if (strcmp(command, "l") == 0) {
            int N = 0, M = 0;
            char path[MAX];
            // citire parametrii de comanda
            scanf("%d%d", &N, &M);
            scanf("%s", path);
            // incarcare imagine in memorie
            if (num_matrix == 0) {
                // construim primul element din vectorul de dimensiuni
                matrix_dim = (int **)malloc(sizeof(int *));
                matrix_dim[0] = (int*)malloc(2 * sizeof(int));
                matrix_dim[0][0] = N;
                matrix_dim[0][1] = M;
                // construim primul element din vector
                matrix_array = (int ****)malloc(sizeof(int ***));
                matrix_array[num_matrix] = matrix_malloc(N, M);
                // citim imaginea si o scriem in matricea nou creata
                read_from_bmp(matrix_array[num_matrix], N, M, path);
                num_matrix++;
            } else {
                // mai adaugam un element in vectorul de dimensiuni folosind un pointer aditional p
                int **p = (int **)realloc(matrix_dim, (num_matrix + 1) * sizeof(int *));
                matrix_dim = p;
                matrix_dim[num_matrix] = (int*)malloc(2 * sizeof(int));
                matrix_dim[num_matrix][0] = N;
                matrix_dim[num_matrix][1] = M;
                // mai adaugam un element in vectorul de matrici folosind un pointer aditional q
                int ****q = (int ****)realloc(matrix_array, (num_matrix + 1) * sizeof(int ***));
                matrix_array = q;
                matrix_array[num_matrix] = matrix_malloc(N, M);
                // citim imaginea si o scriem in matricea nou creata
                read_from_bmp(matrix_array[num_matrix], N, M, path);
                num_matrix++;
            }
        }
        // Save
        if (strcmp(command, "s") == 0) {
            int index  = 0;
            char path[MAX];
            // citim parametrii de comanda
            scanf("%d", &index);
            scanf("%s", path);
            // salvam imaginea la adresa path
            write_to_bmp(matrix_array[index], matrix_dim[index][0], matrix_dim[index][1], path);
        }
        // Apply Horizontal Flip
        if (strcmp(command, "ah") == 0) {
            int index = 0;
            // citim parametrii de comanda
            scanf("%d", &index);
            // aplicam functia de oglindire orizontala si salvam rezultatul intr-un pointer aditional q
            int ***q = flip_horizontal(matrix_array[index], matrix_dim[index][0], matrix_dim[index][1]);
            matrix_array[index] = q;
        }
        // Apply Rotate
        if (strcmp(command, "ar") == 0) {
            int index = 0;
            // citim parametrii de comanda
            scanf("%d", &index);
            /* aplicam functia de rotire cu 90 de grade spre stanga si salvam rezultatul intr-un pointer aditional q;
            interschimbam si dimensiunile matrcei din vector*/
            int ***q = rotate_left(matrix_array[index], matrix_dim[index][0], matrix_dim[index][1]);
            swap_0(&matrix_dim[index][0], &matrix_dim[index][1]);
            matrix_array[index] = q;
        }
        // Apply Crop
        if (strcmp(command, "ac") == 0) {
            int index = 0, x = 0, y = 0, w = 0, h = 0;
            // citim parametrii de comanda
            scanf("%d%d%d%d%d", &index, &x, &y, &w, &h);
            /* aplicam functia crop si salvam rezultatul intr-un pointer aditional q,
            modificand dimensiunile matricei din vector */
            int ***q = crop(matrix_array[index], matrix_dim[index][0], matrix_dim[index][1], x, y, h, w);
            matrix_dim[index][0] = h;
            matrix_dim[index][1] = w;
            matrix_array[index] = q;
        }
        // Apply Extend
        if (strcmp(command, "ae") == 0) {
            int index = 0, rows = 0, cols = 0, R = 0, G = 0, B = 0;
            // citim parametrii de comanda
            scanf("%d%d%d%d%d%d", &index, &rows, &cols, &R, &G, &B);
            /* aplicam functia extend si salvam rezultatul intr-un pointer aditional,
            modificand dimensiunile matricei din vector */
            int ***q = extend(matrix_array[index], matrix_dim[index][0], matrix_dim[index][1], rows, cols, R, G, B);
            matrix_dim[index][0] += 2 * rows;
            matrix_dim[index][1] += 2 * cols;
            matrix_array[index] = q;
        }
        // Apply Paste
        if (strcmp(command, "ap") == 0) {
            int index_dst = 0, index_src = 0, x = 0, y  = 0;
            // citim parametrii de comanda
            scanf("%d%d%d%d", &index_dst, &index_src, &x, &y);
            int N_dst = matrix_dim[index_dst][0];
            int M_dst = matrix_dim[index_dst][1];
            int N_src = matrix_dim[index_src][0];
            int M_src = matrix_dim[index_src][1];
            // aplicam functia paste si salvam rezultatul intr-un pointer aditional
            int ***q = paste(matrix_array[index_dst], N_dst, M_dst, matrix_array[index_src], N_src, M_src, x, y);
            matrix_array[index_dst] = q;
        }
        // Create filter
        if (strcmp(command, "cf") == 0) {
            int size = 0;
            // citire parametrii
            scanf("%d", &size);
            if (num_filters == 0) {
                // construim primul element din vectorul ce retine size-ul filtrului
                filter_size = (int *)malloc(sizeof(int));
                filter_size[num_filters] = size;
                // construim primul element din vectorul de filtre
                filter_array = (float ***)malloc(sizeof(float **));
                filter_array[num_filters] = filter_malloc(size);
                num_filters++;
            } else {
                // mai adaugam un element in vectorul ce retine size ul filtrului
                int *p = (int *)realloc(filter_size, (num_filters + 1) * sizeof(int));
                filter_size = p;
                filter_size[num_filters] = size;
                // mai adaugam un element in vectorul de filtre
                float ***q = (float ***)realloc(filter_array, (num_filters + 1) * sizeof(float **));
                filter_array = q;
                filter_array[num_filters] = filter_malloc(size);
                num_filters++;
            }
            // introducem valorile citite in matricea asociata filtrului
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    scanf("%f", &filter_array[num_filters - 1][i][j]);
                }
            }
        }
        // Apply filter
        if (strcmp(command, "af") == 0) {
            int index_img = 0, index_filter = 0;
            // citire parametrii
            scanf("%d%d", &index_img, &index_filter);
            int N = matrix_dim[index_img][0];
            int M = matrix_dim[index_img][1];
            float **f = filter_array[index_filter];
            // aplicam functia apply_filter si salvam rezultatul intr-un pointer aditional
            int ***q = apply_filter(matrix_array[index_img], N, M, f, filter_size[index_filter]);
            matrix_array[index_img] = q;
        }
        // Delete filter
        if (strcmp(command, "df") == 0) {
            int index_filter = 0;
            // citire parametrii
            scanf("%d", &index_filter);
            // salvam intr-un pointer adresa matricei filtrului ce trebuie sters si o dezalocam
            float **filter_delete = filter_array[index_filter];
            for (int i = 0; i < filter_size[index_filter]; i++) {
                free(filter_delete[i]);
            }
            free(filter_delete);
            // facem shift la stanga cu restul filtrelor
            for (int i = index_filter + 1; i < num_filters; i++) {
                filter_array[i - 1] = filter_array[i];
                filter_size[i - 1] = filter_size[i];
            }
            // realocarea vectorului de filtre
            float ***q = (float ***)realloc(filter_array, (num_filters - 1) * sizeof(float **));
            filter_array = q;
            int *p = (int *)realloc(filter_size, (num_filters - 1) * sizeof(int));
            filter_size = p;
            num_filters--;
        }
        // Delete image
        if (strcmp(command, "di") == 0) {
            int index_img = 0;
            // citire parametrii
            scanf("%d", &index_img);
            // salvam intr-un pointer adresa matricei imaginii ce trebuie sterse si o dezalocam
            int ***matrix_delete = matrix_array[index_img];
            matrix_free(matrix_delete, matrix_dim[index_img][0], matrix_dim[index_img][1]);
            free(matrix_dim[index_img]);
            // facem shift la stanga cu restul imaginilor
            for (int i = index_img + 1; i < num_matrix; i++) {
                matrix_array[i - 1] = matrix_array[i];
                matrix_dim[i - 1] = matrix_dim[i];
            }
            // realocam vectorul de matrici asociate imaginilor
            int ****q = (int ****)realloc(matrix_array, (num_matrix - 1) * sizeof(int **));
            matrix_array = q;
            int **p = (int **)realloc(matrix_dim, (num_matrix - 1) * sizeof(int *));
            matrix_dim = p;
            num_matrix--;
        }
    }
    return 0;
}
