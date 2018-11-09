#ifndef _SCAN_H_
#define _SCAN_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdbool.h>

// calcul de la boîte englobante d'un polygone
int getXmin(poly *polygone);
int getYmin(poly *polygone);
int getXmax(poly *polygone);
int getYmax(poly *polygone);

// teste l'intersection entre 2 segments et stocke ses coordonnées si elle existe
int get_line_intersection(int Ax, int Ay, int Bx, int By, int Cx, int Cy, int Dx, int Dy, int *Ix, int *Iy);

// remplissage d'un polygone
void scan_line(Image *img, poly *polygone);

#endif
