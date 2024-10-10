#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define MAXCOLOR 255

// functie de interschimbare valori
void swap_0(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

// functie de alocare a matricei asociate unei imagini
int ***matrix_malloc(int N, int M) {
    int ***m = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        m[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    return m;
}

// functie de alocare a matricei asociate unui filtru
float **filter_malloc(int size) {
    float **filter = malloc(sizeof(float *) * size);
    for (int i = 0; i < size; i++) {
        filter[i] = malloc(sizeof(float) * size);
    }
    return filter;
}

// functie de dezalocare a matricei asociate unui imagini
void matrix_free(int ***m, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(m[i][j]);
        }
        free(m[i]);
    }
    free(m);
}

// Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    int i = 0, j = 0, k = 0;
    // se face interschimbarea elementelor matricei simetrice dupa axa verticala
    for (i = 0; i < N; i++) {
        for (j = 0; j < M / 2; j++) {
            for (k = 0; k < 3; k++) {
                swap_0(&image[i][j][k], &image[i][M - j - 1][k]);
            }
        }
    }
    return image;
}

// Task 2
int ***rotate_left(int ***image, int N, int M) {
    int i = 0, j = 0, k = 0;
    // alocam o noua matrice de dimensiune M x N
    int ***m = matrix_malloc(M, N);
    // facem transpusa matricei image si apoi o oglindire pe orizontala
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            for (k = 0; k < 3; k++) {
                m[j][i][k] = image[i][j][k];
            }
        }
    }
    // se face interschimbarea elementelor matricei simetrice dupa axa orizontala
    for (i = 0; i < M / 2; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < 3; k++) {
                swap_0(&m[i][j][k], &m[M - i - 1][j][k]);
            }
        }
    }
    // dezalocam matricea initiala
    matrix_free(image, N, M);
    return m;
}

// Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int i = 0, j = 0, k = 0;
    // alocam o noua matrice de dimensiune h x w
    int ***m = matrix_malloc(h, w);
    // copiem doar elementele ce pornesc din punctul (x, y) in noua matrice creata
    for (i = y; i < y + h; i++) {
        for (j = x; j < x + w; j++) {
            for (k = 0; k < 3; k++) {
                m[i - y][j - x][k] = image[i][j][k];
            }
        }
    }
    // dezalocam matricea initiala
    matrix_free(image, N, M);
    return m;
}

// Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int i = 0, j = 0, k = 0;
    // alocam o noua matrice de dimensiune (N + 2 * rows) x (M + 2 * cols)
    int ***m = matrix_malloc(N + 2 * rows, M + 2 * cols);
    // copiem matricea veche in mijlocul matricei noi
    for (i = rows; i < N + rows; i++) {
        for (j = cols; j < M + cols; j++) {
            for (k = 0; k < 3; k++) {
                m[i][j][k] = image[i - rows][j - cols][k];
            }
        }
    }
    // populam restul matricei cu pixeli de aceeasi culoare
    for (i = 0; i < rows; i++) {
        for (j = 0; j < M + 2 * cols; j++) {
            m[i][j][0] = new_R;
            m[i][j][1] = new_G;
            m[i][j][2] = new_B;
        }
    }
    for (i = N + rows; i < N + 2 * rows; i++) {
        for (j = 0; j < M + 2 * cols; j++) {
            m[i][j][0] = new_R;
            m[i][j][1] = new_G;
            m[i][j][2] = new_B;
        }
    }
    for (j = 0; j < cols; j++) {
        for (i = 0; i < N + 2 * rows; i++) {
            m[i][j][0] = new_R;
            m[i][j][1] = new_G;
            m[i][j][2] = new_B;
        }
    }
    for (j = M + cols; j < M + 2 * cols; j++) {
        for (i = 0; i < N + 2 * rows; i++) {
            m[i][j][0] = new_R;
            m[i][j][1] = new_G;
            m[i][j][2] = new_B;
        }
    }
    // dezalocam matricea initiala
    matrix_free(image, N, M);
    return m;
}

// Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
    // copiem pixelii si testam ca acestia sa nu depaseasca limitele matricei destinatie
    int i = 0, j = 0, k = 0;
    for (i = y; (i < y + N_src) && (i < N_dst); i++) {
        for (j = x; (j < x + M_src) && (j < M_dst); j++) {
            for (k = 0; k < 3; k++) {
                image_dst[i][j][k] = image_src[i - y][j - x][k];
            }
        }
    }
    return image_dst;
}

// Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int i = 0, j = 0, k = 0;
    // alocam o noua matrice de dimensiune N x M
    int ***m = matrix_malloc(N, M);
    // construim pentru fiecare culoare a unui pixel suma rezultata in urma aplicarii filtrului folosind formula de suma
    int a = 0, b = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            for (k = 0; k < 3; k++) {
                /* variabilele disi si disj sunt folosite pentru a determina elementul din matrice vecin cu pixelul curent ce trebuie inmultit cu 
                corespondentul din matricea asociata unui filtru */
                int disi = -filter_size / 2;
                int disj = -filter_size / 2;
                float s = 0;
                /* trecem prin fiecare element R(ij) al matricei asociate unui filtru 
                si realizam inmultirea cu vecinul aferent */
                for (a = 0; a < filter_size; a++) {
                    for (b = 0; b < filter_size; b++) {
                        int ifiltru = i + disi + a;
                        int jfiltru = j + disj + b;
                        // testam daca vecinii unui pixel exista sau nu pentru a calcula suma
                        if (((ifiltru >= 0) && (ifiltru < N)) && ((jfiltru >= 0) && (jfiltru < M))) {
                            s += (float)(image[ifiltru][jfiltru][k]) * filter[a][b];
                        }
                    }
                }
                // modificam suma pentru a se incadra in limitele [0, 255]
                if (s < 0)
                    s = 0;
                if (s > MAXCOLOR)
                    s = MAXCOLOR;
                // cast la (int) al sumei
                m[i][j][k] = (int)s;
            }
        }
    }
    // dezalocam matricea initiala
    matrix_free(image, N, M);
    return m;
}
