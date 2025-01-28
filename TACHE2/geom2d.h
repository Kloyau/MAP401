#ifndef _GEOM2D_H_
#define _GEOM2D_H_

#include <math.h>

/*---------------- Structures ----------------*/

/* Définition d'un vecteur en 2D */
typedef struct Vecteur_ {
    double x, y;
} Vecteur;

/* Définition d'un point en 2D */
typedef struct Point_ {
    double x, y;
} Point;

/*---------------- Prototypes des fonctions ----------------*/

/* Création de vecteurs */
Vecteur new_vect_xy(double x, double y);
Vecteur new_vect_2pts(Point p1, Point p2);
Vecteur somme_vect(Vecteur v1, Vecteur v2);
Vecteur aVect(double a, Vecteur v1);

/* Opérations sur les vecteurs */
double pscalaire(Vecteur v1, Vecteur v2);
double len(Vecteur v1);

/* Création de points */
Point new_pts(double x, double y);

/* Opérations sur les points */
Point somme_pts(Point p1, Point p2);
Point aPts(double a, Point p1);
double dist(Point p1, Point p2);

#endif /* _GEOM2D_H_ */