# Rapport de Projet Final MAP401 — Extraction et simplification de contours

Nom : Karl Loyau
Date de présentation : 15 Avril 2025

## Introduction

Ce projet s'inscrit dans le cadre du module MAP401 et a pour objectif la réalisation d’un programme permettant :

1. Extraction des contours d’une image binaire au format PBM (Tâche 5),

2. Simplification de ces contours (Tâche 6),

3. Approximation par des courbes de Bézier (Tâche 7).

## Structure 

*Le projet est constitué d’un module principal unique contours.c, appuyé par deux fichiers de fonctions image.c et geom2d.c.*

### Modules :

image.c : Fonction de manipulation d'images, fournis par le projet

geom2d.c : Manipulation de points, vecteurs, calculs géométriques.

contours.c : Extraction, simplification, écriture EPS, programme principal.

## Tâche 5 — Extraction des contours

### Objectif :

Parcours d’une image PBM et détection des contours noirs à l’aide d’un robot explorateur.

### Approche :

Détection du pixel de départ dans un masque (premier pixel noir d’un composant).

Déplacement du robot selon l’algorithme en orientation (Nord, Est, Sud, Ouest).

Stockage des points du contour dans une liste chaînée.

### Sortie :

Fichier contours.eps contenant tous les contours extraits, fermés et remplis.

## Tâche 6 — Simplification des contours

### Objectif :

Réduire le nombre de points du contour tout en conservant sa forme.

### Algorithme :

Application d’une méthode de distance point-segment grâce à geom2d.c.

Maintien des points extrêmes des segments suffisamment rectilignes.

### Seuil réglable :

Le programme génère les fichiers **simplifiesX.eps** pour chaque seuil X entre 1 et  10.

### Sortie :

Fichiers EPS, visualisant les contours simplifiés et remplis.

## Tâche 7 — Approximation par courbes de Bézier

### Objectif :

Remplacer les segments simplifiés par des courbes de Bézier cubiques pour lisser les contours.

### Méthode :

Interpolation par segments successifs.

Approximation par 2 points de contrôle internes pour chaque segment.

### Sortie :

Fichiers bezierX.eps, un par valeur de seuil.

## Manuel utilisateur

### Compilation

```make```

Exécution

```./contours <nom_image.pbm> <seuil_max>```

Entrée :

Une image binaire .pbm

Un seuil de simplification (entier, entre 1 et 10)

Sortie :

**contours.eps** : contours bruts

**simplifiesX.eps** : contours simplifiés (1 ≤ X ≤ seuil)

**bezierX.eps** : version lissée par courbes de Bézier (1 ≤ X ≤ seuil)