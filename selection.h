#ifndef _SELECT_H_
#define _SELECT_H_

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdbool.h>

void select_point(Image *img, poly *polygone, int position);
void move_point(Image *img, poly  *polygone, int position, char direction);
void delete_point(Image *img, poly *polygone, int position);

void select_edge(Image *img, poly *polygone, int position, bool is_poly);
void add_point_middle_edge(Image *img, poly *polygone, int position);

int closestVertex(Image *img, poly *polygone, int x, int y);
int closestEdge(Image *img, poly *polygone, int x, int y);

#endif
