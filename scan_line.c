#include "Image.h"
#include "bresenham.h"
#include "polygone.h"
#include "scan_line.h"

int getXmin(poly *polygone) {
  int min;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      min = polygone->first->p.x;
      node *current_node = polygone->first;

      // parcours des segments du polygone
      while(current_node->next != NULL) {
        if(current_node->p.x < min) {
            min = current_node->p.x;
        }
        current_node = current_node->next;
      }

      // comparaison du dernier point
      if(polygone->last->p.x < min) {
        min = polygone->last->p.x;
      }
    }
  }

  return min;
}

int getYmin(poly *polygone) {
  int min;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      min = polygone->first->p.y;
      node *current_node = polygone->first;

      // parcours des segments du polygone
      while(current_node->next != NULL) {
        if(current_node->p.y < min) {
            min = current_node->p.y;
        }
        current_node = current_node->next;
      }

      // comparaison du dernier point
      if(polygone->last->p.y < min) {
        min = polygone->last->p.y;
      }
    }
  }

  return min;
}


int getXmax(poly *polygone) {
  int max;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      max = polygone->first->p.x;
      node *current_node = polygone->first;

      // parcours des segments du polygone
      while(current_node->next != NULL) {
        if(current_node->p.x > max) {
            max = current_node->p.x;
        }
        current_node = current_node->next;
      }

      // comparaison du dernier point
      if(polygone->last->p.x > max) {
        max = polygone->last->p.x;
      }
    }
  }

  return max;
}

int getYmax(poly *polygone) {
  int max;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      max = polygone->first->p.y;
      node *current_node = polygone->first;

      // parcours des segments du polygone
      while(current_node->next != NULL) {
        if(current_node->p.y > max) {
            max = current_node->p.y;
        }
        current_node = current_node->next;
      }

      // comparaison du dernier point
      if(polygone->last->p.y > max) {
        max = polygone->last->p.y;
      }
    }
  }

  return max;
}

/**
 * Détecte une intersection, et stocke ses coordonnées dans les derniers paramètres
 * Params:
 * A(x, y) et B(x, y) le premier segment
 * C(x, y) et D(x, y) le deuxième segment
 * I(x, y) les pointeurs où stocker les coordonnées d'intersection
 */
int get_line_intersection(int Ax, int Ay, int Bx, int By, int Cx, int Cy, int Dx, int Dy, int *Ix, int *Iy)
{
  float ABx, ABy, DCx, DCy, CAx, CAy;
  float s, t, d;

  ABx = (float)Bx - (float)Ax;
  ABy = (float)By - (float)Ay;
  DCx = (float)Cx - (float)Dx;
  DCy = (float)Cy - (float)Dy;
  CAx = (float)Ax - (float)Cx;
  CAy = (float)Ay - (float)Cy;

  // calcul du déterminant
  d = DCy*ABx - ABy*DCx;

  // il y a intersection si déterminant != 0
  if(d != 0) {
    s = (DCx*CAy - CAx*DCy) / d;
    t = (CAx*ABy - ABx*CAy) / d;

    // on vérifie que l'intersection se situe sur les segments
    if ((s >= 0.0) && (s <= 1.0) && (t >= 0.0) && (t <= 1.0))
    {
        if (Ix != NULL) {
          *Ix = (int)(Ax + (s*ABx));
        }
        if (Iy != NULL) {
          *Iy = (int)(Ay + (s*ABy));
        }
        return 1;
    }
  }

  return 0;
}

/**
 * Remplissage d'un polygone avec un algorithme scan line
 * Params:
 * Image
 * Polygone
 */
void scan_line(Image *img, poly *polygone) {
  // stockage des intersections
  poly *liste_inter = create_polygone();
  int *inter_x = malloc(sizeof(int));
  int *inter_y = malloc(sizeof(int));

  // boîte englobante
  int Xmin = getXmin(polygone);
  int Ymin = getYmin(polygone);
  int Xmax = getXmax(polygone);
  int Ymax = getYmax(polygone);

  // segment de la largeur de la boîte
  int xA = Xmin;
  int xB = Xmax;
  int yA, yB, has_intersection;

  // parcours de la boîte en hauteur
  for(int i=Ymin; i<Ymax; i++) {
    // AB descend (ligne de scan)
    yA = i;
    yB = i;

    *inter_x = -1;
    *inter_y = -1;

    if(polygone != NULL) {
      if(polygone->first != NULL) {
        node *current_node = polygone->first;

        // parcours des segments du polygone
        while(current_node->next != NULL) {

          // test d'intersection entre la ligne de "scan" et le segment du polygone
          has_intersection = get_line_intersection(
            xA, yA,
            xB, yB,
            current_node->p.x, current_node->p.y,
            current_node->next->p.x, current_node->next->p.y,
            inter_x, inter_y
          );

          // on stocke l'intersection si elle existe
          if(has_intersection) {
            insert_order(liste_inter, *inter_x, *inter_y);
          }

          current_node = current_node->next;
        }


        // le segment last->first ne fait pas partie du polygone, on le teste ici
        has_intersection = get_line_intersection(
          xA, yA,
          xB, yB,
          polygone->last->p.x, polygone->last->p.y,
          polygone->first->p.x, polygone->first->p.y,
          inter_x, inter_y
        );

        if(has_intersection) {
          insert_order(liste_inter, *inter_x, *inter_y);
        }
      }
    }
  }

  //parcours de la liste des intersections pour tracer les lignes de remplissage
  I_polygone(img, liste_inter);
}
