#include "Image.h"
#include "bresenham.h"
#include "scan_line.h"

int getXmin(line *l) {
  int min = l->points[0];

  for(int i=2; i<l->nb_valeurs; i+=2) {
    if(l->points[i] < min) {
      min = l->points[i];
    }
  }

  return min;
}

int getYmin(line *l) {
  int min = l->points[1];

  for(int i=3; i<l->nb_valeurs-1; i+=2) {
    if(l->points[i] < min) {
      min = l->points[i];
    }
  }

  return min;
}

int getXmax(line *l) {
  int max = l->points[0];

  for(int i=2; i<l->nb_valeurs; i+=2) {
    if(l->points[i] > max) {
      max = l->points[i];
    }
  }

  return max;
}

int getYmax(line *l) {
  int max = l->points[1];

  for(int i=3; i<l->nb_valeurs-1; i+=2) {
    if(l->points[i] > max) {
      max = l->points[i];
    }
  }

  return max;
}

bool isInPoly(int x, int y, line *l) {
  // pour chaque segment, fait un test d'intersection
  return false;
}

bool intersect(int xA, int yA, int xB, int yB, int xC, int yC, int xD, int yD) {


  return false;
}

bool pointInSegment(int xA, int yA, int xB, int yB, int x, int y) {
  return (pointsAligned(xA, yA, xB, yB, x, y)
    && pointBetween(xA, yA, xB, yB, x, y));
}

// ?
bool pointsAligned(int xA, int yA, int xB, int yB, int xC, int yC) {
  // 3 points sont alignés si les vecteurs AB et AC sont colinéaires

  // calcul des vecteurs
  int xAB = xB-xA;
  int yAB = yB-yA;
  int xAC = xC-xA;
  int yAC = yC-yA;

  // teste proportionnalité
  return (xAB*yAC == yAB*xAC);
}

bool pointBetween(int xA, int yA, int xB, int yB, int xC, int yC) {

  // 1 point C est entre 2 points A et B si :

  // le produit scalaire AB.AC > 0
  // le produit scalaire AB.AB > AB.AC

  // calcul des vecteurs
  int xAB = xB-xA;
  int yAB = yB-yA;
  int xAC = xC-xA;
  int yAC = yC-xA;

  // calcul des produits scalaires
  int scal_AB_AC = xAB*xAC + yAB*yAC;

  if(scal_AB_AC <= 0) {
    return false;
  }

  int scal_AB_AB = xAB*xAB + yAB*yAB;

  if(scal_AB_AB < scal_AB_AC) {
    return false;
  }

  return true;
}
