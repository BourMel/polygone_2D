#include "Image.h"
#include "bresenham.h"
#include "scan_line.h"
//
// int getXmin(line *l) {
//   int min = l->points[0];
//
//   for(int i=2; i<l->nb_valeurs; i+=2) {
//     if(l->points[i] < min) {
//       min = l->points[i];
//     }
//   }
//
//   return min;
// }
//
// int getYmin(line *l) {
//   int min = l->points[1];
//
//   for(int i=3; i<l->nb_valeurs-1; i+=2) {
//     if(l->points[i] < min) {
//       min = l->points[i];
//     }
//   }
//
//   return min;
// }
//
// int getXmax(line *l) {
//   int max = l->points[0];
//
//   for(int i=2; i<l->nb_valeurs; i+=2) {
//     if(l->points[i] > max) {
//       max = l->points[i];
//     }
//   }
//
//   return max;
// }
//
// int getYmax(line *l) {
//   int max = l->points[1];
//
//   for(int i=3; i<l->nb_valeurs-1; i+=2) {
//     if(l->points[i] > max) {
//       max = l->points[i];
//     }
//   }
//
//   return max;
// }

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
int get_line_intersection(int Ax, int Ay, int Bx, int By,
    int Cx, int Cy, int Dx, int Dy, int *i_x, int *i_y)
{
    float ABx, ABy, DCx, DCy, CAx, CAy;
    ABx = (float)Bx - (float)Ax;
    ABy = (float)By - (float)Ay;
    DCx = (float)Cx - (float)Dx;
    DCy = (float)Cy - (float)Dy;
    CAx = (float)Ax - (float)Cx;
    CAy = (float)Ay - (float)Cy;

    float s, t, d;

    d = DCy * ABx - ABy * DCx;
    if(d != 0) //intersection si determinant != 0
    {
      s = (DCx * CAy - CAx * DCy) / d;
      t = (CAx * ABy - ABx * CAy) / d;
      if (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0)
      {
          // Collision detected
          if (i_x != NULL)
              *i_x = (int)(Ax + (s * ABx));
          if (i_y != NULL)
              *i_y = (int)(Ay + (s * ABy));
          return 1;
      }
    }

    return 0; // No collision
}

void scan_line(Image *img, poly *polygone) {
  poly *liste_inter;

  liste_inter = malloc(sizeof(struct struct_polygone));
  if(liste_inter == NULL) {
    exit(EXIT_FAILURE);
  }

  liste_inter->nb = 0;
  liste_inter->first = NULL;
  liste_inter->last = NULL;

  // à corriger avec les valeurs de la boîte englobante
  int Xmin = 0;
  int Ymin = 0;
  int Xmax = 399;
  int Ymax = 399;
  int xA = Xmin;
  int xB = Xmax;
  int *inter_x = malloc(sizeof(int));
  int *inter_y = malloc(sizeof(int));
  int yA, yB, has_intersection;
  // parcourir la fenêtre en hauteur
  for(int i=Ymin; i<Ymax; i++) {
    // segment AB de toute la largeur de la fenêtre
    yA = i;
    yB = i;

    *inter_x = -1;
    *inter_y = -1;
    // parcourt des segments du polygone
    if(polygone != NULL) {
      if(polygone->first != NULL) {
        node *current_node = polygone->first;

        while(current_node->next != NULL) {

          has_intersection = get_line_intersection(
            xA, yA,
            xB, yB,
            current_node->p.x, current_node->p.y,
            current_node->next->p.x, current_node->next->p.y,
            inter_x, inter_y
          );
          // on stocke les intersections
          if(has_intersection) {
            //printf("Inter\n");
            //printf("%d, %d\n",*inter_x, *inter_y );
            insert_order(liste_inter, *inter_x, *inter_y);
          }

          current_node = current_node->next;
        }
        //tester dernier segment entre last et first
        has_intersection = get_line_intersection(
          xA, yA,
          xB, yB,
          polygone->last->p.x, polygone->last->p.y,
          polygone->first->p.x, polygone->first->p.y,
          inter_x, inter_y
        );
        if(has_intersection) {
          //printf("Inter\n");
          //printf("%d, %d\n",*inter_x, *inter_y );
          insert_order(liste_inter, *inter_x, *inter_y);
        }
      }
    }
  }

  display(liste_inter);
  //parcourt des intersections pour tracer les lignes de remplissage
  if(liste_inter != NULL) {
    if(liste_inter->first != NULL) {
      node *current_node = liste_inter->first;

      while(current_node->next != NULL) {
        //printf("%d,%d\n",current_node->p.x,current_node->p.y );
        I_bresenham(
          img,
          current_node->p.x,
          current_node->p.y,
          current_node->next->p.x,
          current_node->next->p.y
        );
        current_node = current_node->next;
      }
    }
  }
}
