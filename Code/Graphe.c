

#include"Graphe.h"

/*Creation et Liberation*/
/*Creation Arete*/
// Crée une nouvelle arête avec les sommets u et v
Arete* creerArete(int u, int v){
    Arete *a = (Arete*)malloc(sizeof(Arete));
    if(a == NULL){
        printf("Erreur lors de l'allocation mémoire pour la création d'une arête\n");
        return NULL;
    }
    a->u = u;
    a->v = v;

    return a;
}

// Crée une nouvelle cellule d'arête contenant l'arête donnée
Cellule_arete* creerCellule_arete(Arete *a){
    if(a == NULL){
        printf("Impossible de créer une cellule d'arête pour une arête inexistante\n");
        return NULL;
    }

    Cellule_arete* c = (Cellule_arete*)malloc(sizeof(Cellule_arete));
    if(c == NULL){
        printf("Erreur lors de l'allocation mémoire pour la création d'une cellule d'arête\n");
        return NULL;
    }

    c->a = a;
    c->suiv = NULL;

    return c;
}

// Libère la mémoire allouée pour une cellule d'arête
void libererCellule_Arete(Cellule_arete *c){
    if(c == NULL){
        printf("La cellule d'arête est déjà libérée\n");
        return;
    }

    c->a = NULL;
    c->suiv = NULL;

    free(c);
}


// Crée un nouveau sommet avec le numéro, les coordonnées x et y spécifiés.
Sommet* creerSommet(int num, double x, double y){
    Sommet* s = (Sommet*)malloc(sizeof(Sommet));
    if(s==NULL){
        printf("Erreur malloc création sommet\n");
        return NULL;
    }
    s->num = num;
    s->x = x;
    s->y = y;
    s->L_voisin = NULL;

    return s;
}

/* Libère la mémoire allouée pour un sommet */
void libererSommet(Sommet *s){
    if(s==NULL){
        printf("Sommet déjà libéré\n");
        return;
    }

    s->L_voisin = NULL;
    free(s);
}

/* Met à jour les arêtes d'un sommet à partir des voisins d'un noeud */
void majAretes(Graphe *g, Sommet *s, Noeud *n){
    // Boucle passant par tous les voisins du noeud
    CellNoeud *nvois = n->voisins;
    while(nvois){
        // On cherche le sommet correspondant au noeud voisin
        Sommet *svois = g->T_som[((nvois->nd->num)-1)];

        // Si le sommet correspondant au voisin a été créé, on ajoute l'arête
        // Sinon, ce sommet sera créé plus tard
        if(svois){
            // Création de l'arête
            Arete *a = creerArete(s->num, svois->num);
            // Création des cellules d'arête pour chaque sommet
            Cellule_arete *c = creerCellule_arete(a);
            Cellule_arete *cvois = creerCellule_arete(a);

            // Ajout de l'arête en tête de liste pour le sommet actuel
            c->suiv = s->L_voisin;
            s->L_voisin = c;

            // Ajout de l'arête en tête de liste pour le sommet voisin
            cvois->suiv = svois->L_voisin;
            svois->L_voisin = cvois;
        }

        nvois = nvois->suiv;
    }
}


Graphe* creerGraphe(Reseau* r){
    // Vérifie si le réseau existe
    if(r == NULL){
        printf("Impossible de créer un graphe à partir d'un réseau inexistant\n");
        return NULL;
    }

    // Alloue de la mémoire pour le graphe
    Graphe *g = (Graphe*)malloc(sizeof(Graphe));
    if(g==NULL){
        printf("Erreur lors de l'allocation mémoire pour la création du graphe\n");
        return NULL;
    }

    // Initialise les variables du graphe
    g->nbsom = r->nbNoeuds;
    g->gamma = r->gamma;
    g->nbcommod = nbCommodites(r);

    // Alloue de la mémoire pour le tableau de sommets
    g->T_som = (Sommet**)malloc(sizeof(Sommet*) * g->nbsom);
    if(g->T_som == NULL){
        printf("Erreur lors de l'allocation mémoire pour le tableau de sommets du graphe\n");
        free(g);
        return NULL;
    }
    // Initialise chaque élément du tableau à NULL
    for(int i=0; i < g->nbsom; i++){
        g->T_som[i] = NULL;
    }

    // Alloue de la mémoire pour le tableau de commodités
    g->T_commod = (Commod*)malloc(sizeof(Commod) * g->nbcommod);
    if(g->T_commod == NULL){
        printf("Erreur lors de l'allocation mémoire pour le tableau de commodités du graphe\n");
        free(g->T_som);
        free(g);
        return NULL;
    }

    // Boucle parcourant tous les nœuds du réseau
    CellNoeud *cr = r->noeuds;
    int num;
    for(int i=0; i < g->nbsom; i++){
        // Crée un sommet correspondant à chaque nœud du réseau
        num = (cr->nd->num) - 1; // Le numéro du sommet correspond à la position dans le tableau
        g->T_som[num] = creerSommet(num, cr->nd->x, cr->nd->y);
        
        // Met à jour les arêtes du sommet
        majAretes(g, g->T_som[num], cr->nd);

        cr = cr->suiv;
    }

    // Boucle parcourant toutes les commodités du réseau
    CellCommodite *Cr = r->commodites;
    for(int i=0; i<(g->nbcommod); i++){
        // Initialise les commodités à partir des extrémités des commodités du réseau
        Commod C;
        C.e1 = (Cr->extrA->num)-1;
        C.e2 = (Cr->extrB->num)-1;

        g->T_commod[i] = C;
        Cr = Cr->suiv;
    }

    return g;
}

// Libère la mémoire allouée pour un graphe
void libererGraphe(Graphe *g){
    if(g==NULL){
        printf("Le graphe est déjà libéré\n");
        return;
    }

    Sommet *s;
    for(int i=0; i < g->nbsom; i++){
        s = g->T_som[i];
        Cellule_arete *ctemp, *c = s->L_voisin;
        while(c){
            // Vérifie si l'arête a déjà été libérée
            if((c->a->u <= i) && (c->a->v <= i)){
                free(c->a);
            }
            ctemp = c;
            c = c->suiv;
            libererCellule_Arete(ctemp);
        }

        libererSommet(s);
    }

    free(g->T_som);
    free(g->T_commod);

    free(g);
}
