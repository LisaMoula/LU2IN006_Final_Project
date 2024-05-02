//MOULA Lisa 21215357
//DUPART Guillaume 28710554


#ifndef __RESEAU_H__
#define __RESEAU_H__

#include "Chaine.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;               /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv;         /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y);
Reseau* reconstitueReseauListe(Chaines *C);
void ecrireReseau(Reseau *R, FILE *f);
int nbLiaisons(Reseau *R);
int nbCommodites(Reseau *R);
void afficheReseauSVG(Reseau *R, char* nomInstance);

Noeud* creer_Noeud(int num, double x, double y);
void liberer_Noeud(Noeud* n);
CellNoeud* creer_CellNoeud(Noeud* n);
void liberer_CellNoeud(CellNoeud *c);
CellCommodite* creer_CellCommodite(Noeud* extrA, Noeud* extrB);
void liberer_CellCommodite(CellCommodite *C);
Reseau* creer_Reseau(int gamma);
void liberer_Reseau(Reseau *R);
void maj_voisins(Reseau *R, Noeud *v, Noeud *n);

#endif

