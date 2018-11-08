#include "Image.h"
#include "bresenham.h"

/**
 * A partir de 2 points quelconques, cette fonction utilise la symétrie
 * pour se ramener dans le premier octant
 * Params:
 * xA & yA coordonnées du point A
 * xB & yB coordonnées du point B
 * xA_1o & yA_1o des adresses pour stocker les nouvelles coordonnées de A (1er octant)
 * xB_1o & yB_1o des adresses pour stocker les nouvelles coordonnées de B (1er octant)
 */

void Z2_to_octant1(int xA, int yA, int xB, int yB, int *xA_1o, int *yA_1o, int *xB_1o, int *yB_1o) {
  int xA_1q, xB_1q, yA_1q, yB_1q;

  if(xB > xA) {
    xA_1q = xA;
    xB_1q = xB;
  // on opère une symétrie sur Oy
  } else {
    xA_1q = - xA;
    xB_1q = - xB;
  }

  if(yB > yA) {
    yA_1q = yA;
    yB_1q = yB;
  // on opère une symétrie sur Ox
  } else {
    yA_1q = - yA;
    yB_1q = - yB;
  }

  // si l'on est dans le premier octant (pente < 1)
  if((xB_1q - xA_1q) > (yB_1q - yA_1q)) {
    *xA_1o = xA_1q;
    *xB_1o = xB_1q;
    *yA_1o = yA_1q;
    *yB_1o = yB_1q;
  // si l'on est dans le 2e octant
  } else {
    *xA_1o = yA_1q;
    *xB_1o = yB_1q;
    *yA_1o = xA_1q;
    *yB_1o = xB_1q;
  }
}

/**
 * A partir de 2 points A et B appartenant à un octant O, cette fonction
 * ramène le point P(x, y) du premier octant vers l'octant O
 * Params:
 * xA & yA coordonnées du point A
 * xB & yB coordonnées du point B
 * xA_1o & yA_1o les coordonnées actuelles de P (dans le premier octant)
 * x & y les adresses où stocker les coordonnées d'origine de P
 */

void octant1_to_Z2 (int xA, int yA, int xB, int yB, int x_1o, int y_1o, int *x, int *y) {
  int x_1q, y_1q;

  // si la pente de AB est inférieure à 1 (premier octant)
  if(abs(xB - xA) > abs(yB - yA)) {
    x_1q = x_1o;
    y_1q = y_1o;
  } else {
    x_1q = y_1o;
    y_1q = x_1o;
  }

  if(xB > xA) {
    *x = x_1q;
  // on opère une symétrie sur Oy
  } else {
    *x = -x_1q;
  }

  if(yB > yA) {
    *y = y_1q;
  // on opère une symétrie sur Ox
  } else {
    *y = -y_1q;
  }
}

/**
 * Dessine, en blanc, une droite de Bresenham
 * Params:
 * img l'image dans laquelle dessiner
 * xA & yA les coordonnées du premier point A de la droite
 * xB & yB les coordonnées du deuxième point B de la droite
 */
void I_bresenham(Image *img, int xA, int yA, int xB, int yB) {
  Color c = C_new(255, 255, 255);

  int xA_1o, yA_1o, xB_1o, yB_1o, x_Z2, y_Z2;

  // on se ramène d'abord dans le premier octant
  Z2_to_octant1(xA, yA, xB, yB, &xA_1o, &yA_1o, &xB_1o, &yB_1o);

  // et on utilise les nouvelles coordonnées de A et B pour faire les calculs
  int dx = xB_1o - xA_1o;
  int dy = yB_1o - yA_1o;
  int incrd1 = 2*dy;
  int incrd2 = 2*(dy-dx);

  int x = xA_1o;
  int y = yA_1o;
  int d = 2*dy-dx;

  while(x < xB_1o) {
    // avant l'affichage, on quitte le premier octant
    octant1_to_Z2(xA, yA, xB, yB, x, y, &x_Z2, &y_Z2);
    I_plotColor(img, x_Z2, y_Z2, c);

    x++;

    if(d<0) {
      d += incrd1;
    } else {
      y++;
      d += incrd2;
    }
  }
}

/**
 * A partir d'un tableau de coordonnées, trace une ligne brisée reliant
 * un ensemble de points
 * Params:
 * Image dans laquelle dessiner
 * Tableau contenant des paires de coordonnées (x, y)
 * Taille du tableau
 */
void I_ligne_brisee(Image *img, poly *polygone) {

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      node *current_node = polygone->first;

      while(current_node->next != NULL) {
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


/**
 * Création d'un nouveau point dans une structure polygone
 */
 void insert(poly *polygone, int x, int y)
 {
   node *new_node = malloc(sizeof(struct struct_node));
   if(new_node == NULL) {
     exit(EXIT_FAILURE);
   }

   point new_point = {x, y};

   new_node->p = new_point;

   //on met le point à la suite du polygone
   new_node->next = NULL;

   // on ajoute new_node à la liste chaînée
   if(polygone->nb == 0) {
     polygone->first = new_node;
     polygone->last = new_node;

   } else {
     polygone->last->next = new_node;
     polygone->last = new_node;
   }

   polygone->nb++;
 }

 /**
  * Insertion d'un point en respectant l'ordre des x
  * dans une structure de type polygone
  */
void insert_order(poly *polygone, int x, int y) {
  node *new_node = malloc(sizeof(struct struct_node));
  if(new_node == NULL) {
    exit(EXIT_FAILURE);
  }

  point new_point = {x, y};
  new_node->next = NULL;
  new_node->p = new_point;

  if(polygone != NULL) {
    // si vide
    if(polygone->nb == 0) {
      polygone->first = new_node;
      polygone->last = new_node;
    }
    if(polygone->nb == 1) {
      if(x >= polygone->first->p.x) {
        polygone->first->next = new_node;
        polygone->last = new_node;
      }
      else {
        new_node->next = polygone->first;
        polygone->first = new_node;
      }
    } else {
      // parcourt
      if(polygone->first != NULL) {
        node *current_node = polygone->first;

        while(current_node->next != NULL) {
          if(x >= current_node->p.x
          && x < current_node->next->p.x) {
            new_node->next = current_node->next;
            current_node->next = new_node;
            break;
          } else {
            current_node = current_node->next;
          }
        }
      }
    }
  }

  polygone->nb++;
}
