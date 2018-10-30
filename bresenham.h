#ifndef _BRESEN_H_
#define _BRESEN_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

typedef struct struct_line {
  int nb_valeurs; // conserve le nombre de valeurs allouées (2 par point)
  int points[100]; // tableau d'au maximum 100 valeurs, soit 50 points
} line;

// fonctions auxiliaires
void Z2_to_octant1(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o);
void octant1_to_Z2 (int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y);

// tracé d'une droite de Bresenham
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);

// tracé d'une ligne brisée reliant un ensemble de points
void I_ligne_brisee(Image *img, int tableau[], int taille);

// ajout à un point d'une structure "line"
void add_point_to_line(line *l, int x, int y);


#endif
