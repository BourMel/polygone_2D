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
