#ifndef _BRESEN_H_
#define _BRESEN_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

typedef struct struct_point {
  int x, y;
} point;

typedef struct struct_node {
  point p;
  struct struct_node *next;
} node;

typedef struct struct_polygone {
    size_t nb;
    node *first;
    node *last;
} poly;

// fonctions auxiliaires
void Z2_to_octant1(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o);
void octant1_to_Z2 (int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y);

// tracé d'une droite de Bresenham
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);

// tracé d'une ligne brisée reliant un ensemble de points
void I_ligne_brisee(Image *img, poly *polygone);

// @TODO
void insert(poly *polygone, int x, int y);
void insert_order(poly *polygone, int x, int y);

// void display_ligne_brisee(int tableau[], int taille);

// ajout à un point d'une structure "line"
// void add_point_to_line(line *l, int x, int y);
// void remove_point_from_line(line *l);


#endif
