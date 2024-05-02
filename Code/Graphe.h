//MOULA Lisa 21215357
//DUPART Guillaume 28710554

#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include<stdlib.h>
#include<stdio.h>
#include"ArbreQuat.h"
#include"Struct_File.h"

typedef struct{
    int u, v; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete{
    Arete *a; /* pointeur sur l’arete */
    struct cellule_arete *suiv;
} Cellule_arete;

typedef struct{
    int num; /* Numero du sommet (le meme que dans T_som) */
    double x, y;
    Cellule_arete *L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
    int e1 ,e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct{
    int nbsom; /* Nombre de sommets */
    Sommet **T_som; /* Tableau de pointeurs sur sommets */
    int gamma;
    int nbcommod; /* Nombre de commodites */
    Commod *T_commod; /* Tableau des commodites */
} Graphe;

Arete* creerArete(int u, int v);
Cellule_arete* creerCellule_arete(Arete *a);
Sommet* creerSommet(int num, double x, double y);
Graphe* creerGraphe(Reseau* r);

void libererCellule_Arete(Cellule_arete *c);
void libererSommet(Sommet *s);
void libererGraphe(Graphe *g);

/*Question 7.2 et 7.3*/
S_file* ppChemin(Graphe *g, int sd, int sf);

/*Question 7.4*/
int** creerMatrice(int nbLignes, int nbColonnes);
void libererMatrice(int** matrice, int nbLignes);
int reorganiseReseau(Reseau *r);

#endif