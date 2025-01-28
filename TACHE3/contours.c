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

void trouver_contours(Image im){
    printf("Debut de la recherche de countours\n");

    Pixel pg,pd;
    Point p0 = trouver_pixel_depart(im);
    robot rob= {p0.x-1,p0.y-1, Est};
    int stop=FALSE;
    while (!stop){

        printf("Orientation: %d, Position: (%d, %d)\n", rob.orient, rob.x, rob.y);

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
}

int main(){
    char* nom_f = "prout.pbm";
    Image im = lire_fichier_image(nom_f);
    trouver_contours(im);
    return 0;
}