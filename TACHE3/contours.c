#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "geom2d.h"

#define MAX_SEUIL 10


typedef enum {Nord, Est, Sud, Ouest} orientation;
typedef struct robot_ {
    int x, y;
    orientation orient;
} robot;

typedef struct Tableau_Point_ {
    unsigned int taille;  /* nombre d'éléments dans le tableau */
    Point *tab;           /* tableau des points */
} Tableau_Point;

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

double distance_point_segment(Point P, Point A, Point B) {
    double ABx = B.x - A.x, ABy = B.y - A.y;
    double APx = P.x - A.x, APy = P.y - A.y;
    double ab2 = ABx * ABx + ABy * ABy;
    double t = ab2 == 0 ? 0 : fmax(0, fmin(1, (APx * ABx + APy * ABy) / ab2));
    double closestX = A.x + t * ABx, closestY = A.y + t * ABy;
    return hypot(P.x - closestX, P.y - closestY);
}

Liste_Point simplifier_contour_depuis_eps(Liste_Point L, double seuil) {
    Tableau_Point TP = sequence_points_liste_vers_tableau(L);
    Liste_Point Ls = creer_liste_Point_vide();

    if (TP.taille < 2) return Ls;

    int i = 0;
    Ls = ajouter_element_liste_Point(Ls, TP.tab[0]);
    while (i < TP.taille - 1) {
        int j = i + 1;
        while (j < TP.taille) {
            int valide = 1;
            for (int k = i + 1; k < j; k++) {
                if (distance_point_segment(TP.tab[k], TP.tab[i], TP.tab[j]) > seuil) {
                    valide = 0;
                    break;
                }
            }
            if (!valide) break;
            j++;
        }
        j--;
        Ls = ajouter_element_liste_Point(Ls, TP.tab[j]);
        i = j;
    }
    free(TP.tab);
    return Ls;
}
void ecrire_bezier_eps(Tableau_Point TP, FILE *fp, int hauteur) {
    if (TP.taille < 2) return;

    fprintf(fp, "%d %d moveto\n", (int)TP.tab[0].x, hauteur - (int)TP.tab[0].y);

    for (int i = 0; i < TP.taille - 1; i++) {
        Point A = TP.tab[i];
        Point B = TP.tab[i + 1];

        Point C1 = { A.x + (B.x - A.x) / 3.0, A.y + (B.y - A.y) / 3.0 };
        Point C2 = { A.x + 2 * (B.x - A.x) / 3.0, A.y + 2 * (B.y - A.y) / 3.0 };

        fprintf(fp, "%d %d %d %d %d %d curveto\n",
                (int)C1.x, hauteur - (int)C1.y,
                (int)C2.x, hauteur - (int)C2.y,
                (int)B.x,  hauteur - (int)B.y);
    }

    fprintf(fp, "closepath\nfill\n");
}
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image.pbm>\n", argv[0]);
        return 1;
    }

    const double seuil_simplification = 1.5;

    Image im = lire_fichier_image(argv[1]);
    Image M = creer_masque(im);
    int nbContours = 0, total_segments = 0;
    int largeur = largeur_image(im), hauteur = hauteur_image(im);
    Liste_Point contours[100];
    Liste_Point contours_simplifies[100];

    while (1) {
        Liste_Point c = trouver_contours(im, M);
        if (c.taille == 0) break;
        contours[nbContours++] = c;
    }

    // Tâche 5 : Contours bruts
    FILE *fp = fopen("contours.eps", "w");
    fprintf(fp, "%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n", largeur, hauteur);
    for (int i = 0; i < nbContours; i++) {
        Tableau_Point TP = sequence_points_liste_vers_tableau(contours[i]);
        total_segments += TP.taille - 1;
        if (TP.taille > 0) {
            fprintf(fp, "%d %d moveto ", (int)TP.tab[0].x, hauteur - (int)TP.tab[0].y);
            for (int j = 1; j < TP.taille; j++)
                fprintf(fp, "%d %d lineto ", (int)TP.tab[j].x, hauteur - (int)TP.tab[j].y);
            fprintf(fp, "closepath\n");
        }
        free(TP.tab);
    }
    fprintf(fp, "fill\nshowpage\n");
    fclose(fp);

    printf("Tâche 5 : %d contours extraits, %d segments au total\n", nbContours, total_segments);

    // Tâche 6 : Simplification
    FILE *fp_s = fopen("contours_simplifies.eps", "w");
    fprintf(fp_s, "%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n", largeur, hauteur);

    int total_simplified_segments = 0;
    for (int i = 0; i < nbContours; i++) {
        contours_simplifies[i] = simplifier_contour_depuis_eps(contours[i], seuil_simplification);
        Tableau_Point TP = sequence_points_liste_vers_tableau(contours_simplifies[i]);
        total_simplified_segments += TP.taille - 1;
        if (TP.taille > 0) {
            fprintf(fp_s, "%d %d moveto ", (int)TP.tab[0].x, hauteur - (int)TP.tab[0].y);
            for (int j = 1; j < TP.taille; j++)
                fprintf(fp_s, "%d %d lineto ", (int)TP.tab[j].x, hauteur - (int)TP.tab[j].y);
            fprintf(fp_s, "closepath\nfill\n");
        }
        free(TP.tab);
    }
    fprintf(fp_s, "showpage\n");
    fclose(fp_s);

    printf("Tâche 6 : %d segments après simplification (réduction %.2f%%)\n",
            total_simplified_segments,
            100.0 * (total_segments - total_simplified_segments) / total_segments);

    // Tâche 7 : Bézier (à partir de la version simplifiée)
    FILE *fp_b = fopen("contours_bezier.eps", "w");
    fprintf(fp_b, "%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n", largeur, hauteur);

    int total_beziers = 0;
    for (int i = 0; i < nbContours; i++) {
        Tableau_Point TP = sequence_points_liste_vers_tableau(contours_simplifies[i]);
        if (TP.taille >= 4) {
            fprintf(fp_b, "%d %d moveto\n", (int)TP.tab[0].x, hauteur - (int)TP.tab[0].y);
            for (int j = 1; j + 2 < TP.taille; j += 3) {
                fprintf(fp_b, "%d %d %d %d %d %d curveto\n",
                        (int)TP.tab[j].x, hauteur - (int)TP.tab[j].y,
                        (int)TP.tab[j+1].x, hauteur - (int)TP.tab[j+1].y,
                        (int)TP.tab[j+2].x, hauteur - (int)TP.tab[j+2].y);
                total_beziers++;
            }
            fprintf(fp_b, "closepath\nfill\n");
        }
        free(TP.tab);
    }

    fprintf(fp_b, "showpage\n");
    fclose(fp_b);

    printf("Tâche 7 : %d courbes de Bézier cubiques générées\n", total_beziers);

    return 0;
}
