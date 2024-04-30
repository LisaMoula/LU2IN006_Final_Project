

#include"Graphe.h"

/*Creation et Liberation*/
/*Creation Arete*/
Arete* creerArete(int u, int v){
    Arete *a = (Arete*)malloc(sizeof(Arete));
    if(a==NULL){
        printf("Erreur malloc création Arete\n");
        return NULL;
    }
    a->u = u;
    a->v = v;

    return a;
}
/*Liberation d'une arete est un simple free*/

/*Creation Cellule_arete*/
Cellule_arete* creerCellule_arete(Arete *a){
    if(a==NULL){
        printf("Impossible de creer Cellule_arete pour arete qui n'existe pas\n");
        return NULL;
    }

    Cellule_arete* c = (Cellule_arete*)malloc(sizeof(Cellule_arete));
    if(c==NULL){
        printf("Erreur malloc creation Cellule_arete\n");
        return NULL;
    }

    c->a = a;
    c->suiv = NULL;

    return c;
}

/*Liberation Cellule_arete*/
void libererCellule_Arete(Cellule_arete *c){
    if(c==NULL){
        printf("Cellule_arete deja libérée\n");
        return;
    }

    c->a = NULL;
    c->suiv = NULL;

    free(c);
}

/*Creation Sommet*/
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

/*Liberation Sommet*/
void libererSommet(Sommet *s){
    if(s==NULL){
        printf("Sommet deja libere\n");
        return;
    }

    s->L_voisin = NULL;
    free(s);
}

/*Mise a jour des aretes d'un sommet*/
void majAretes(Graphe *g, Sommet *s, Noeud *n){
    /*Boucle passant par tous les voisins du noeud*/
    CellNoeud *nvois = n->voisins;
    while(nvois){
        /*On cherche le sommet correspondant au noeud voisin*/
        Sommet *svois = g->T_som[((nvois->nd->num)-1)];

            /*Si le sommet correspondant au voisin a ete créé, on ajoute l'arete
            Sinon, ce sommet sera créé plus tard*/
            if(svois){
                /*On sait que l'arete n'existe pas car le nouveau sommet viens d'etre créé
                Nous créons donc l'arete et nous l'ajoutons en tete de liste (des deux sommets!)*/
                Arete *a = creerArete(s->num, svois->num);
                Cellule_arete *c = creerCellule_arete(a);
                Cellule_arete *cvois = creerCellule_arete(a);

                c->suiv = s->L_voisin;
                s->L_voisin = c;

                cvois->suiv = svois->L_voisin;
                svois->L_voisin = cvois;
            }

        nvois = nvois->suiv;
    }
}

/*Creation Graphe*/
Graphe* creerGraphe(Reseau* r){
    if(r == NULL){
        printf("Impossible de creer Graphe a partir d'un reseau qui n'existe pas\n");
        return NULL;
    }

    Graphe *g = (Graphe*)malloc(sizeof(Graphe));
    if(g==NULL){
        printf("Erreur malloc création Graphe\n");
        return NULL;
    }

    /*Initialisation des variables de g*/
    g->nbsom = r->nbNoeuds;
    g->gamma = r->gamma;
    g->nbcommod = nbCommodites(r);

    g->T_som = (Sommet**)malloc(sizeof(Sommet*) * g->nbsom);
    if(g->T_som == NULL){
        printf("Erreur malloc T_som création Graphe\n");
        free(g);
        return NULL;
    }
    for(int i=0; i < g->nbsom; i++){
        g->T_som[i] = NULL;
    }

    g->T_commod = (Commod*)malloc(sizeof(Commod) * g->nbcommod);
    if(g->T_commod == NULL){
        printf("Erreur malloc T_commod création Graphe\n");
        free(g->T_som);
        free(g);
        return NULL;
    }

    /*Boucle qui parcours tous les noeuds du Reseau*/
    CellNoeud *cr = r->noeuds;
    int num;
    for(int i=0; i < g->nbsom; i++){
        /*Creation du sommet correspondant au noeud*/
        /*Le numero du noeud (-1 car ils commencent a 1) correspond au numero du sommet, et donc de sa position dans le tableau*/
        num = (cr->nd->num) - 1;
        g->T_som[num] = creerSommet(num, cr->nd->x, cr->nd->y);
        
        /*Mise a jour aretes*/
        majAretes(g, g->T_som[num], cr->nd);

        cr = cr->suiv;
    }

    /*Boucle passant par toutes les commodites du Reseau*/
    CellCommodite *Cr = r->commodites;
    for(int i=0; i<(g->nbcommod); i++){
        Commod C;
        C.e1 = (Cr->extrA->num)-1;
        C.e2 = (Cr->extrB->num)-1;

        g->T_commod[i] = C;
        Cr = Cr->suiv;
    }

    return g;
}

void libererGraphe(Graphe *g){
    if(g==NULL){
        printf("Graphe deja libere\n");
        return;
    }

    Sommet *s;
    for(int i=0; i < g->nbsom; i++){
        s = g->T_som[i];
        Cellule_arete *ctemp, *c = s->L_voisin;
        while(c){
            /*Pour ne pas liberer 2 fois la meme arete, on verifie si nous allons la rencontrer plus tard ou pas*/
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