/*---------------- Dépendances ----------------*/

#include <stdio.h>
#include <math.h>


/*---------------- Structures ----------------*/

typedef struct Vecteur_{
  double x,y;
} Vecteur;

typedef struct Point_{
  double x,y;
} Point;


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

int main(){
  Vecteur v1 = {2.0, 3.5};
  Vecteur v2 = {0.0, 2.0};
  Point p1 = {5.0, 6.5};
  Point p2 = {2.0, 1.5};

  Vecteur v3 = new_vect_xy(1.0, 2.0);
  printf("new_vect_xy: (%.2f, %.2f)\n", v3.x, v3.y);

  Vecteur v4 = new_vect_2pts(p1, p2);
  printf("new_vect_2pts: (%.2f, %.2f)\n", v4.x, v4.y);

  Vecteur v5 = somme_vect(v1, v2);
  printf("somme_vect: (%.2f, %.2f)\n", v5.x, v5.y);

  Vecteur v6 = aVect(2.0, v1);
  printf("aVect: (%.2f, %.2f)\n", v6.x, v6.y);

  double scal = pscalaire(v1, v2);
  printf("pscalare: %.2f\n", scal);

  double length = len(v1);
  printf("len: %.2f\n", length);

  Point p3 = new_pts(3.0, 4.0);
  printf("new_pts: (%.2f, %.2f)\n", p3.x, p3.y);

  Point p4 = somme_pts(p1, p2);
  printf("somme_pts: (%.2f, %.2f)\n", p4.x, p4.y);

  Point p5 = aPts(2.0, p1);
  printf("aPts: (%.2f, %.2f)\n", p5.x, p5.y);

  double distance = dist(p1, p2);
  printf("dist: %.2f\n", distance);

  return 0;

}
