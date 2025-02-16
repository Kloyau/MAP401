#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"image.h"
#include"geom2d.h"


typedef enum {Nord, Est, Sud, Ouest} orientation;
typedef struct robot_ {
    int x,y;
    orientation orient;

} robot;

typedef struct Tableau_Point_
{
	unsigned int taille; /* nombre d'elements dans le tableau */
	Point *tab;          /* (pointeur vers) le tableau des elements */
} Tableau_Point;


/*---- le type cellule de liste de point ----*/
typedef struct Cellule_Liste_Point_
{
	Point data;    /* donnee de l'element de liste */
	struct Cellule_Liste_Point_* suiv; /* pointeur sur l'element suivant */
} Cellule_Liste_Point;

Point set_point(double x, double y)
{
	Point P = {x,y}; return P;
}
/*---- le type liste de point ----*/
typedef struct Liste_Point_
{
	unsigned int taille;        /* nombre d'elements dans la liste */
	Cellule_Liste_Point *first; /* pointeur sur le premier element de la liste */
	Cellule_Liste_Point *last;  /* pointeur sur le dernier element de la liste */
	                       /* first = last = NULL et taille = 0 <=> liste vide */
} Liste_Point;

typedef Liste_Point Contour; /* type Contour = type Liste_Point */


/* creer une cellule de liste avec l'element v 
renvoie le pointeur sur la cellule de liste creee
   la fonction s'arrete si la creation n'a pas pu se faire */
Cellule_Liste_Point *creer_element_liste_Point(Point v)
{
	Cellule_Liste_Point *el;
	el = (Cellule_Liste_Point *)malloc(sizeof(Cellule_Liste_Point));
	if (el==NULL)
	{
		fprintf(stderr, "creer_element_liste_Point : allocation impossible\n");
		exit(-1);
	}
	el->data = v;
	el->suiv = NULL;
	return el;
}
/* creer une liste vide */
Liste_Point creer_liste_Point_vide()
{
	Liste_Point L = {0, NULL, NULL};
	return L;
}

/* ajouter l'element e en fin de la liste L, renvoie la liste L modifiee */
Liste_Point ajouter_element_liste_Point(Liste_Point L, Point e)
{
	Cellule_Liste_Point *el;
	
	el = creer_element_liste_Point(e);
	if (L.taille == 0)
	{
		/* premier element de la liste */
		L.first = L.last = el;
	}
	else
	{
		L.last->suiv = el;
		L.last = el;
	}
	L.taille++;
	return L;
}

Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L)
{
	Tableau_Point T;
	
	/* taille de T = taille de L */
	T.taille = L.taille;
	
	/* allocation dynamique du tableau de Point */
	T.tab = malloc(sizeof(Point) * T.taille);
	if (T.tab == NULL)
	{
		/* allocation impossible : arret du programme avec un message */
		fprintf(stderr, "sequence_points_liste_vers_tableau : ");
		fprintf(stderr, " allocation impossible\n");
		exit(-1);
	}
	
	/* remplir le tableau de points T en parcourant la liste L */
	int k = 0; /* indice de l'element dans T.tab */
	Cellule_Liste_Point *el = L.first; /* pointeur sur l'element dans L */
	while (el) 
	{
		T.tab[k] = el->data;
		k++; /* incrementer k */
		el = el->suiv; /* passer a l'element suivant dans la liste chainee */
	}
	
	return T;
}

/* ecrire le contour L a l'ecran 
cette fonction montre un exemple de conversion d'une liste de points en
tableau de points afin de pouvoir par la suite acceder aux elements d'une
   sequence de points par indice */
void ecrire_contour(Liste_Point L)
{
	Tableau_Point TP = sequence_points_liste_vers_tableau(L);
	int k;
	int nP = TP.taille;
	
	printf("Contour avec %d points\n[", nP);
	for (k = 0; k < nP; k++)
	{	
		Point P = TP.tab[k]; /* recuperer le point d'indice k */
		printf(" (%5.1f,%5.1f)", P.x, P.y);
	} 
	printf("]\n");
	
	free(TP.tab); /* supprimer le tableau de point TP */
}

Point trouver_pixel_depart(Image I){
    int x = largeur_image(I);
	int y = hauteur_image(I);
	for (int i = 1; i <= x; i++){
		for (int j = 1; j<=y;j++){
			if (get_pixel_image(I,i,j)==NOIR && get_pixel_image(I,i,j-1)==BLANC){
                printf("Pixel de depart trouvé!\n");
                Point p ={i,j};
                return p;
            }
        }
    }
    printf("Aucun point valide trouvé\n");
    Point p ={0,0};
    return p;
}

Liste_Point trouver_contours(Image im){
    printf("Debut de la recherche de countours\n");

    Liste_Point ls = creer_liste_Point_vide();

    Point pt;
    Pixel pg,pd;
    Point p0 = trouver_pixel_depart(im);
    robot rob= {p0.x-1,p0.y-1, Est};
    int stop=FALSE;
    while (!stop){
        pt = set_point(rob.x,rob.y);
        ls = ajouter_element_liste_Point(ls,pt);

        /* Avancer et calcul pixels droite-gauche*/
        if (rob.orient==Nord){
            rob.y --;
            pg = get_pixel_image(im,rob.x,rob.y);
            pd = get_pixel_image(im,rob.x+1,rob.y);
        }
        else if (rob.orient==Est){
            rob.x ++;
            pg = get_pixel_image(im,rob.x+1,rob.y);
            pd = get_pixel_image(im,rob.x+1,rob.y+1);
        }
        else if (rob.orient == Sud){
            rob.y ++;
            pg = get_pixel_image(im,rob.x+1,rob.y+1);
            pd = get_pixel_image(im,rob.x,rob.y+1);
        }
        else{
            rob.x --;
            pg = get_pixel_image(im,rob.x,rob.y+1);
            pd = get_pixel_image(im,rob.x,rob.y);
        }


        /* calculer orientation*/
        if (pg == NOIR){
            if(rob.orient==Nord){
                rob.orient=Ouest;
            }
            else if(rob.orient==Est){
                rob.orient=Nord;
            }
            else if(rob.orient==Sud){
                rob.orient=Est;
            }
            else{
                rob.orient=Sud;
            }
        }
        else if (pd == BLANC){
            if(rob.orient==Nord){
                rob.orient=Est;
            }
            else if(rob.orient==Est){
                rob.orient=Sud;
            }
            else if(rob.orient==Sud){
                rob.orient= Ouest;
            }
            else{
                rob.orient=Nord;
            }
        }
        

        if (rob.x==p0.x-1 && rob.y==p0.y-1 && rob.orient == Est){
            stop = TRUE;
        }
    }
    return ls;
}

int main(){
    char* nom_f = "tete.pbm";
    int largeur,hauteur,x,y,i;
    Image im = lire_fichier_image(nom_f);

    FILE *fp;
    fp = fopen ("contours.eps","w");
    fprintf(fp,"%%! PS−Adobe−3.0 EPSF−3.0\n");
    
    largeur= largeur_image(im);
    hauteur= hauteur_image(im);
    fprintf(fp,"%%%%BoundingBox : 0 0 %i %i\n",largeur,hauteur);
    Tableau_Point Tpts= sequence_points_liste_vers_tableau(trouver_contours(im));
    Point pt=Tpts.tab[0];
    fprintf(fp,"%i %i moveto ",(int)pt.x,hauteur-(int)pt.y);


    for (i=1;i<Tpts.taille;i++){

        pt=Tpts.tab[i];
    
        fprintf(fp,"%i %i lineto ",(int)pt.x,hauteur-(int)pt.y);

    }

    fprintf(fp,"\n0 setlinewidth stroke\nshowpage");
    fclose(fp);


    return 0;
}