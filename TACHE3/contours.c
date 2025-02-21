#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "geom2d.h"

typedef enum {Nord, Est, Sud, Ouest} orientation;
typedef struct robot_ {
    int x, y;
    orientation orient;
} robot;

typedef struct Tableau_Point_ {
    unsigned int taille;  /* nombre d'éléments dans le tableau */
    Point *tab;           /* tableau des points */
} Tableau_Point;

/*---- cellule et liste chaînée de points ----*/
typedef struct Cellule_Liste_Point_ {
    Point data;
    struct Cellule_Liste_Point_* suiv;
} Cellule_Liste_Point;

typedef struct Liste_Point_ {
    unsigned int taille;
    Cellule_Liste_Point *first;
    Cellule_Liste_Point *last;
} Liste_Point;

typedef Liste_Point Contour;

Point set_point(double x, double y) {
    Point P = {x, y};
    return P;
}

Cellule_Liste_Point *creer_element_liste_Point(Point v) {
    Cellule_Liste_Point *el = malloc(sizeof(Cellule_Liste_Point));
    if (el == NULL) {
        fprintf(stderr, "creer_element_liste_Point : allocation impossible\n");
        exit(-1);
    }
    el->data = v;
    el->suiv = NULL;
    return el;
}

Liste_Point creer_liste_Point_vide() {
    Liste_Point L = {0, NULL, NULL};
    return L;
}

Liste_Point ajouter_element_liste_Point(Liste_Point L, Point e) {
    Cellule_Liste_Point *el = creer_element_liste_Point(e);
    if (L.taille == 0) {
        L.first = L.last = el;
    } else {
        L.last->suiv = el;
        L.last = el;
    }
    L.taille++;
    return L;
}

Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L) {
    Tableau_Point T;
    T.taille = L.taille;
    T.tab = malloc(sizeof(Point) * T.taille);
    if (T.tab == NULL) {
        fprintf(stderr, "sequence_points_liste_vers_tableau : allocation impossible\n");
        exit(-1);
    }
    int k = 0;
    Cellule_Liste_Point *el = L.first;
    while (el) {
        T.tab[k++] = el->data;
        el = el->suiv;
    }
    return T;
}

void ecrire_contour(Liste_Point L) {
    Tableau_Point TP = sequence_points_liste_vers_tableau(L);
    printf("Contour avec %d points\n[", TP.taille);
    for (int k = 0; k < TP.taille; k++) {	
        Point P = TP.tab[k];
        printf(" (%5.1f,%5.1f)", P.x, P.y);
    } 
    printf(" ]\n");
    free(TP.tab);
}

Image creer_masque(Image I) {
    UINT L = largeur_image(I);
    UINT H = hauteur_image(I);
    Image M = creer_image(L, H);  /* Masque même dimension que I*/
    for (int y = 1; y <= H; y++) {
        for (int x = 1; x <= L; x++) {
            if (get_pixel_image(I, x, y) == NOIR &&
                (y == 1 || get_pixel_image(I, x, y-1) == BLANC)) {
                set_pixel_image(M, x, y, NOIR);
            } else {
                set_pixel_image(M, x, y, BLANC);
            }
        }
    }
    return M;
}

Point trouver_pixel_depart(Image I, Image M) {
    UINT L = largeur_image(I);
    UINT H = hauteur_image(I);
    for (int y = 1; y <= H; y++) {
        for (int x = 1; x <= L; x++) {
            if (get_pixel_image(M, x, y) == NOIR) {
                printf("Pixel de depart trouve a (%d,%d) via le masque\n", x, y);
                Point p = {x, y};
                return p;
            }
        }
    }
    printf("Aucun pixel candidat trouve dans le masque\n");
    Point p = {0, 0};
    return p;
}

/* Extraction d'un contour unique en utilisant la méthode du robot et le masque M.
Lorsqu'un pixel candidat est utilisé, on le marque dans M en le passant à BLANC.
La conversion des coordonnées du robot en coordonnées d'image se fait en ajoutant 1.
*/
Liste_Point trouver_contours(Image im, Image M) {
    printf("Debut de l'extraction d'un contour via le masque M\n");
    Liste_Point ls = creer_liste_Point_vide();
    Point pt;
    Pixel pg, pd;
    Point p0 = trouver_pixel_depart(im, M);
    if (p0.x == 0 && p0.y == 0)
        return ls;
    
    robot rob = {p0.x - 1, p0.y - 1, Est};
    int stop = 0;
    UINT L_img = largeur_image(im);
    
    while (!stop) {
        pt = set_point(rob.x, rob.y);
        ls = ajouter_element_liste_Point(ls, pt);
        
        set_pixel_image(M, rob.x + 1, rob.y + 1, BLANC);
        
        if (rob.orient == Nord) {
            rob.y--;
            pg = get_pixel_image(im, rob.x, rob.y);
            pd = get_pixel_image(im, rob.x + 1, rob.y);
        } else if (rob.orient == Est) {
            rob.x++;
            pg = get_pixel_image(im, rob.x + 1, rob.y);
            pd = get_pixel_image(im, rob.x + 1, rob.y + 1);
        } else if (rob.orient == Sud) {
            rob.y++;
            pg = get_pixel_image(im, rob.x + 1, rob.y + 1);
            pd = get_pixel_image(im, rob.x, rob.y + 1);
        } else { /* Ouest */
            rob.x--;
            pg = get_pixel_image(im, rob.x, rob.y + 1);
            pd = get_pixel_image(im, rob.x, rob.y);
        }
        
        /* Calcul de la nouvelle orientation selon les valeurs des pixels voisins */
        if (pg == NOIR) {
            if (rob.orient == Nord)
                rob.orient = Ouest;
            else if (rob.orient == Est)
                rob.orient = Nord;
            else if (rob.orient == Sud)
                rob.orient = Est;
            else
                rob.orient = Sud;
        } else if (pd == BLANC) {
            if (rob.orient == Nord)
                rob.orient = Est;
            else if (rob.orient == Est)
                rob.orient = Sud;
            else if (rob.orient == Sud)
                rob.orient = Ouest;
            else
                rob.orient = Nord;
        }
        
        if (rob.x == p0.x - 1 && rob.y == p0.y - 1 && rob.orient == Est)
            stop = 1;
    }
    return ls;
}

int main() {
    char* nom_f = "animaux.pbm";
    int largeur, hauteur, i, total_segments = 0;
    int nbContours = 0;
    Image im = lire_fichier_image(nom_f);
    
    Image M = creer_masque(im);
    
    Liste_Point contours[100];
    
    while (1) {
        Liste_Point contour = trouver_contours(im, M);
        if (contour.taille == 0)
            break;
        contours[nbContours++] = contour;
    }
    printf("Nombre de contours extraits : %d\n", nbContours);
    for (i = 0; i < nbContours; i++) {
        ecrire_contour(contours[i]);
        if (contours[i].taille > 0)
            total_segments += (contours[i].taille - 1);
    }
    printf("Nombre total de segments : %d\n", total_segments);
    
    FILE *fp = fopen("contours.eps", "w");

    fprintf(fp, "%%! PS-Adobe-3.0 EPSF-3.0\n");
    largeur = largeur_image(im);
    hauteur = hauteur_image(im);
    fprintf(fp, "%%%%BoundingBox: 0 0 %i %i\n", largeur, hauteur);
    for (i = 0; i < nbContours; i++) {

        Tableau_Point TP = sequence_points_liste_vers_tableau(contours[i]);

        if (TP.taille > 0) {
            Point pt = TP.tab[0];
            fprintf(fp, "%i %i moveto ", (int)pt.x, hauteur - (int)pt.y);
            for (int j = 1; j < TP.taille; j++) {
                pt = TP.tab[j];
                fprintf(fp, "%i %i lineto ", (int)pt.x, hauteur - (int)pt.y);
            }
            fprintf(fp, "closepath\n");
        }
    }
    fprintf(fp, "\n0 setlinewidth stroke\nshowpage");
    fclose(fp);

    
    return 0;
}