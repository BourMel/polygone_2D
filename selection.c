#include "Image.h"
#include "bresenham.h"
#include "polygone.h"
#include "scan_line.h"

/**
 * Met en valeur un point d'indice donné pour indiquer la sélection.
 * Params:
 * Image dans laquelle dessiner
 * Polygone à parcourir
 * Position du point à sélectionner
 */
void select_point(Image *img, poly *polygone, int position) {
  if(polygone != NULL) {
    if(polygone->first != NULL) {
      int i = 0;
      node *current_node = polygone->first;

      // parcours des points du polygone
      while((i != position) && (current_node->next != NULL)) {
        current_node = current_node->next;
        i++;
      }

      // tracé d'un carré
      // trait du haut
      I_bresenham(
        img,
        current_node->p.x-5, current_node->p.y-5,
        current_node->p.x+5, current_node->p.y-5
      );
      // trait du bas
      I_bresenham(
        img,
        current_node->p.x-5, current_node->p.y+5,
        current_node->p.x+5, current_node->p.y+5
      );
      // trait de gauche
      I_bresenham(
        img,
        current_node->p.x-5, current_node->p.y+5,
        current_node->p.x-5, current_node->p.y-5
      );
      // trait de droite
      I_bresenham(
        img,
        current_node->p.x+5, current_node->p.y+5,
        current_node->p.x+5, current_node->p.y-5
      );
    }
  }
}

/**
 * Déplace le point d'indice "position" du polygone passé en argument
 * Image dans laquelle dessiner
 * Polygone dans lequel se trouve le point
 * Position du point dans le polygone
 * Direction dans laquelle déplacer le point
 */
void move_point(Image *img, poly  *polygone, int position, char direction) {
  if(polygone != NULL) {
    if(polygone->first != NULL) {
      int i = 0;
      node *current_node = polygone->first;

      // parcours des points du polygone
      while((i != position) && (current_node->next != NULL)) {
        current_node = current_node->next;
        i++;
      }

      // déplacement du point
      if(direction == 'd') {
        current_node->p.x += 1;
      } else if(direction == 'g') {
        current_node->p.x -= 1;
      } else if(direction == 'h') {
        current_node->p.y -= 1;
      } else if(direction == 'b') {
        current_node->p.y +=1;
      }
    }
  }
}

/**
 * Supprime un point du polygone selon une position donnée
 * Params:
 * Image dans laquelle dessiner
 * Polygone à parcourir
 * Position du point dans le polygone
 */
void delete_point(Image *img, poly *polygone, int position) {
  if(polygone != NULL) {
    if(polygone->first != NULL) {
      int i = 0;
      node *current_node = polygone->first;

      // premier point
      if(position == 0) {
        polygone->first = current_node->next;
        free(current_node);

      } else {
        // parcours des points du polygone
        while((i != position-1) && (current_node->next != NULL)) {
          i++;
          current_node = current_node->next;
        }

        if(position != polygone->nb) {
          current_node->next = current_node->next->next;
        } else {
          // dernier point
          current_node->next = NULL;
          polygone->last = current_node;
        }
      }

      polygone->nb--;
    }
  }
}

/**
 * Met en valeur une arête d'indice donné pour indiquer la sélection.
 * Params:
 * Image dans laquelle dessiner
 * Polygone à parcourir
 * Position de l'arête à sélectionner
 * Indiquer s'il s'agit d'un polygone ou d'une ligne brisée
 */
void select_edge(Image *img, poly *polygone, int position, bool is_poly) {
  Color c = C_new(0, 255, 255);

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      // cas de l'arête fermante
      if((position == polygone->nb-1) && is_poly) {
        I_bresenham_color(
          img, c,
          polygone->last->p.x, polygone->last->p.y,
          polygone->first->p.x, polygone->first->p.y
        );

      // tous les autres cas
      } else {

        int i = 0;
        node *current_node = polygone->first;

        // parcours des points du polygone
        while((i != position) && (current_node->next != NULL)) {
          current_node = current_node->next;
          i++;
        }

        // mise en valeur grâce à une couleur
        I_bresenham_color(
          img, c,
          current_node->p.x, current_node->p.y,
          current_node->next->p.x, current_node->next->p.y
        );

      }
    }
  }
}

/**
 * Add a point at the middle of an edge
 * Params:
 * Image dans laquelle dessiner
 * Polygone à parcourir
 * Arête concernée
 * @TODO: quand la dernière arête est divisée, le dernier point est ignoré
 * par la sélection - à corriger
 */
void add_point_middle_edge(Image *img, poly *polygone, int position) {
  node *new_node;
  node *next_node;
  float x, y;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      int i = 0;
      node *current_node = polygone->first;

      // parcours des points du polygone
      while((i != position) && (current_node->next != NULL)) {
        current_node = current_node->next;
        i++;
      }

      // premier point de l'arête = dernier point du polygone
      if(i == polygone->nb) {
        next_node = polygone->first;
      } else {
        next_node = current_node->next;
      }

      x = (current_node->p.x + next_node->p.x) / 2;
      y = (current_node->p.y + next_node->p.y) / 2;

      // création du nouveau point
      new_node = malloc(sizeof(struct struct_node));
      if(new_node == NULL) {
        exit(EXIT_FAILURE);
      }

      point new_point = {x, y};
      new_node->p = new_point;

      // insertion dans la liste chaînée
      current_node->next = new_node;
      new_node->next = next_node;
    }
  }
}

/**
 * Retourne l'indice du sommet le plus proche de (x, y)
 * Params:
 * Image dans laquelle dessiner
 * Polygone contenant le sommet
 * Position x
 * Position y
 */
int closestVertex(Image *img, poly *polygone, int x, int y) {
  int distance_min;
  int position;
  int distance;
  int i;

  float x_middle, y_middle;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      i = 0;
      node *current_node = polygone->first;

      // parcours des points du polygone
      while(current_node->next != NULL) {
        // calcule le milieu de l'arête
        x_middle = (current_node->p.x + current_node->next->p.x) / 2;
        y_middle = (current_node->p.y + current_node->next->p.y) / 2;

        // distance entre le milieu de l'arête et les coordonnées passées en paramètre
        distance = sqrt(
          pow(x - x_middle, 2)
          + pow(y - y_middle, 2)
        );

        if((i == 0) || (distance < distance_min)) {
          distance_min = distance;
          position = i;
        }

        current_node = current_node->next;
        i++;
      }

      // comparaison avec le dernier point
      distance = sqrt(
        pow(x - current_node->p.x, 2)
        + pow(y - current_node->p.y, 2)
      );

      if(distance < distance_min) {
        distance_min = distance;
        position = i;
      }
    }
  }

  return position;
}

/**
 * Retourne l'indice de l'arête la plus proche de (x, y)
 * Params:
 * Image dans laquelle dessiner
 * Polygone contenant le sommet
 * Position x
 * Position y
 */
int closestEdge(Image *img, poly *polygone, int x, int y) {
  int distance_min;
  int position;
  int distance;
  int i;

  if(polygone != NULL) {
    if(polygone->first != NULL) {
      i = 0;
      node *current_node = polygone->first;

      // parcours des points du polygone
      while(current_node->next != NULL) {
        // distance entre les points
        distance = sqrt(
          pow(x - current_node->p.x, 2)
          + pow(y - current_node->p.y, 2)
        );

        if((i == 0) || (distance < distance_min)) {
          distance_min = distance;
          position = i;
        }

        current_node = current_node->next;
        i++;
      }

      // comparaison avec le dernier point
      distance = sqrt(
        pow(x - current_node->p.x, 2)
        + pow(y - current_node->p.y, 2)
      );

      if(distance < distance_min) {
        distance_min = distance;
        position = i;
      }
    }
  }

  return position;
}
