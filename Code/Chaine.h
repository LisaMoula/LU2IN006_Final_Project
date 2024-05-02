//MOULA Lisa 21215357
//DUPART Guillaume 28710554


#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
#include<time.h>

#include "SVGwriter.h"

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Cellule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

Chaines* lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);

//Question 1.2
Chaines* creer_chaines(int gamma, int nbChaines);
CellChaine* creer_cellChaine(int numero);
CellPoint* creer_cellPoint(double x, double y);
void liberer_cellPoint(CellPoint* p);
void liberer_cellChaine(CellChaine* c);
void liberer_chaines(Chaines* C);
void ins_en_tete_chaine(Chaines* C, int numero);
void ins_en_tete_point(CellChaine* c, double x, double y);
Chaines* lectureChaines(FILE* f);
void ecrireChaines(Chaines *C, FILE *f);

//Question 1.4
double longueurChaine(CellChaine *c);
/*Question 6.2*/
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax);

#endif	
