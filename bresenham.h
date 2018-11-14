#ifndef _BRESEN_H_
#define _BRESEN_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

typedef struct struct_point {
  int x, y;
} point;

// fonctions auxiliaires de Bresenham
void Z2_to_octant1(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o);
void octant1_to_Z2 (int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y);

// tracé d'une droite de Bresenham
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);
void I_bresenham_color(Image *img, Color c, int xA, int yA, int xB, int yB);

#endif
