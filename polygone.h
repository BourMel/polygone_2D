#ifndef _POLY_H_
#define _POLY_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

typedef struct struct_node {
  point p;
  struct struct_node *next;
} node;

typedef struct struct_polygone {
    size_t nb;
    node *first;
    node *last;
} poly;

poly* create_polygone();
void empty_polygone(poly *polygone);

// tracé du polygone
void I_polygone(Image *img, poly *polygone);

// insertion d'un point à la fin du polygone
void insert(poly *polygone, int x, int y);
// insertion d'un point en respectant un ordre croissant en y
void insert_order(poly *polygone, int x, int y);

void display(poly *polygone);

#endif
