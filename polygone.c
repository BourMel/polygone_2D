#include "Image.h"
#include "bresenham.h"
#include "polygone.h"
#include "scan_line.h"

/**
 * Allocation et initialisation d'un polygone
 */
poly* create_polygone() {
  poly* polygone = malloc(sizeof(struct struct_polygone));
  if(polygone == NULL) {
    exit(EXIT_FAILURE);
  }

  polygone->nb = 0;
  polygone->first = NULL;
  polygone->last = NULL;

  return polygone;
}

/**
 * Supprime l'ensemble des noeuds d'un polygone
 * Params : polygone à supprimer
 */
 void empty_polygone(poly *polygone) {
   if(polygone != NULL) {
       node *current_node = polygone->first;
       node *tmp;

     // parcourt des différents points
     while(current_node->next != NULL) {
       tmp = current_node;
       free(current_node);
       current_node = tmp->next;
     }
   }

   polygone->nb = 0;
   polygone->first = NULL;
   polygone->last = NULL;
 }

/**
 * A partir d'un tableau de coordonnées, trace un polygone
 * un ensemble de points
 * Params:
 * Image dans laquelle dessiner
 * Polygone à tracer
 */
void I_polygone(Image *img, poly *polygone) {
  if(polygone != NULL) {
    if(polygone->first != NULL) {
      node *current_node = polygone->first;

      // parcourt des différents points, et tracé
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
 * Params :
 * Polygone
 * Coordonnée x du nouveau point
 * Coordonnée y du nouveau point
 */
 void insert(poly *polygone, int x, int y)
 {
   node *new_node = malloc(sizeof(struct struct_node));
   if(new_node == NULL) {
     exit(EXIT_FAILURE);
   }

   point new_point = {x, y};
   new_node->p = new_point;

   //on met le point à la fin du polygone
   new_node->next = NULL;

   // on ajoute new_node à la liste chaînée
   if(polygone->nb == 0) {
     polygone->first = new_node;
     polygone->last = new_node;

   } else {
     // on remplace le dernier noeud par le nouveau
     polygone->last->next = new_node;
     polygone->last = new_node;
   }

   polygone->nb++;
 }

/**
 * Affichage des différents points d'un polygone
 * Params:
 * Polygone
 */
void display(poly *polygone) {
  printf("________Polygone________\n");
  if(polygone != NULL) {
    if(polygone->first != NULL) {
      node *current_node = polygone->first;

      if(polygone->nb == 1) {
        printf("(%d, %d) ", current_node->p.x, current_node->p.y);
      }

      while(current_node->next != NULL) {
        printf("(%d, %d) ", current_node->p.x, current_node->p.y);
        current_node = current_node->next;
      }

      // affichage du dernier point (à cause de la condition du while)
      printf("(%d, %d) ", current_node->p.x, current_node->p.y);
    }
  }
  printf("\n________________\n");
}

/**
  * Insertion d'un point en respectant l'ordre des x
  * dans une structure de type polygone
  * Params :
  * Polygone
  * Coordonnée x du nouveau point
  * Coordonnée y du nouveau point
  */
void insert_order(poly *polygone, int x, int y) {
  node *new_node = malloc(sizeof(struct struct_node));
  if(new_node == NULL) {
    exit(EXIT_FAILURE);
  }

  // création du noeud contenant le point
  point new_point = {x, y};
  new_node->next = NULL;
  new_node->p = new_point;

  if(polygone != NULL) {

    // si le polygone est vide
    if(polygone->nb == 0) {
      polygone->first = new_node;
      polygone->last = new_node;

    // si le polygone ne contient qu'un seul noeud
    } else if(polygone->nb == 1) {

      // si x est supérieur à la valeur existante
      if(x >= polygone->first->p.x) {
        // y est inséré à la fin
        polygone->first->next = new_node;
        polygone->last = new_node;

      } else {
        // x est inséré au début
        new_node->next = polygone->first;
        polygone->first = new_node;
      }

    // polygone de taille quelconque
    } else {

      if(polygone->first != NULL) {
        node *current_node = polygone->first;

        // parcours du polygone
        while(current_node->next != NULL) {

          // insertion de x entre la valeur précédente et la valeur suivante
          if((x >= current_node->p.x) && (x <= current_node->next->p.x)) {
            new_node->next = current_node->next;
            current_node->next = new_node;
            break;

          // insertion de y au début du polygone (seul cas où y < current_node)
          } else if(x <= current_node->p.x) {
            new_node->next = polygone->first;
            polygone->first = new_node;
            break;

          // insertion de x à la fin du polygone
          } else if(current_node->next->next == NULL) {
            current_node->next->next = new_node;
            polygone->last = new_node;
            break;

          // si x n'est pas inséré, on continue le parcours
          } else {
            current_node = current_node->next;
          }
        }
      }
    }
  }

  polygone->nb++;
}
