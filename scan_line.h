#ifndef _SCAN_H_
#define _SCAN_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdbool.h>

// int getXmin(line *l);
// int getYmin(line *l);
// int getXmax(line *l);
// int getYmax(line *l);

// compte les intersections d'une droite avec un polygone
// bool isInPoly(int x, int y, line *l);

// teste l'intersection entre deux droites
bool intersect(int xA, int yA, int xB, int yB, int xC, int yC, int xD, int yD);
// indique si un point se trouve sur un segment
bool pointInSegment(int xA, int yA, int xB, int yB, int x, int y);

// teste si 3 points sont alignés
bool pointsAligned(int xA, int yA, int xB, int yB, int xC, int yC);
// teste si un point est entre deux autre
bool pointBetween(int xA, int yA, int xB, int yB, int xC, int yC);

int get_line_intersection(int p0_x, int p0_y, int p1_x, int p1_y,
    int p2_x, int p2_y, int p3_x, int p3_y, int *i_x, int *i_y);

void scan_line(Image *img, poly *polygone);

#endif
