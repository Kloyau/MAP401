/*---------------- Dépendances ----------------*/

#include <stdio.h>
#include <math.h>
#include "geom2d.h"

/*---------------- Structures ----------------*/


/*---------------- Vecteurs ----------------*/
Vecteur new_vect_xy(double x, double y){
  /*
  Crée un nouveau vecteur à partir de deux coordonnées.

  Entrée : (double) x, (double) y
  Sortie : (Vecteur) v(x,y)
  */
  Vecteur v = {x,y};
  return v;
}

Vecteur new_vect_2pts(Point p1, Point p2){
  /*
  Crée un nouveau vecteur à partir de deux points.

  Entrée : (Point) p1, (Point) p2
  Sortie : (Vecteur) v(x2 - x1, y2 - y1)
  */
  Vecteur v = {p2.x-p1.x, p2.y-p1.y};
  return v;
}

Vecteur somme_vect (Vecteur v1, Vecteur v2){
  /*
  Calcule la Somme de deux Vecteurs v1 et v2
  
  Entrée : (Vecteur) v1, (Vecteur) v2
  Sortie : (Vecteur) v l'addition de v1 et v2 
  */
  double x = v1.x + v2.x;  
  double y = v1.y + v2.y;  
  Vecteur v = {x,y};
  return v;
}

Vecteur aVect (double a, Vecteur v1){
  /*
  Multiplie un Vecteur v1 par un scalaire a

  Entrée : (double) a, (Vecteur) v1
  Sortie : (Vecteur) v la multiplication de v1 par a
  */
  double x = a*v1.x;
  double y = a*v1.y;
  Vecteur v = {x,y};
  return v;
}

double pscalaire (Vecteur v1, Vecteur v2){
  /*
  Calcule le produit scalaire de v1 et v2

  Entrée : (Vecteur) v1, (Vecteur) v2
  Sortie : (double) scal le produit scalaire
  */
  double scal = v1.x*v2.x + v1.y*v2.y;
  return scal;
}

double len(Vecteur v1){
  /*
  Calcule la norme euclidienne (longueur) du Vecteur v1

  Entrée : (Vecteur) v1
  Sortie : (double) len la longueur de v1
  */
  double len = sqrt(v1.x*v1.x + v1.y*v1.y);
  return len;
}


/*---------------- Points ----------------*/

Point new_pts (double x, double y){
  /*
  Crée un nouveau point à partir de deux coordonnées.

  Entrée : (double) x, (double) y
  Sortie : (Point) p(x,y)
  */
  Point p = {x,y};
  return p;
}

Point somme_pts (Point p1, Point p2){
  /*
  Calcule la Somme de deux Points p1 et p2

  Entrée : (Point) p1, (Point) p2
  Sortie : (Point) p l'addition de p1 et p2 
  */
  double x = p1.x + p2.x;
  double y = p1.y + p2.y;
  Point p = {x,y};
  return p;
}

Point aPts (double a, Point p1){
  /*
  Multiplie un Point p1 par un scalaire a

  Entrée : (double) a, (Point) p1
  Sortie : (Point) p la multiplication de p1 par a
  */
  double x = a*p1.x;
  double y = a*p1.y;
  Point p = {x,y};
  return p;
}

double dist(Point p1, Point p2){
  /*
  Calcule la distance entre deux Points

  Entrée : (Point) p1, (Point) p2
  Sortie : (double) dist la distance entre p1 et p2 
  */
  double dist = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
  return dist;
}


/*---------------- Tests ----------------*/
