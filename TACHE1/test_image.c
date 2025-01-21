#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"image.h"

int main(){
    char* nom_f = "fein.pbm";
    Image im = lire_fichier_image(nom_f);
    ecrire_image(im);
    printf("------------------\n");
    ecrire_image(negatif_image(im));
    return 0;
}

