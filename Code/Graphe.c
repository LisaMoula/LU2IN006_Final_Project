//Lisa MOULA 21215357
//Guillaume DUPART 28710554

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

/*Question 7.2*/
S_file* ppChemin(Graphe *g, int u, int v){
    /*Si le sommet de depart est le meme que le sommet final*/
    if(u==v){
        printf("ppChemin: Sommet de départ et d'arrivée est le même\n");
        return NULL;
    }

    /*Creation tableau distance*/
    int* distance = (int*)malloc(sizeof(int)*(g->nbsom));
    if(distance==NULL){
        printf("Erreur malloc tableau distance ppChemin\n");
        return NULL;
    }
    for(int i=0; i<(g->nbsom); i++){
        distance[i] = 0;
    }

    /*Creation tableau visite*/
    int* visite = (int*)malloc(sizeof(int)*(g->nbsom));
    if(visite==NULL){
        printf("Erreur malloc tableau visite ppChemin\n");
        free(distance);
        return NULL;
    }
    for(int i=0; i<(g->nbsom); i++){
        visite[i] = 0;
    }

    /*Creation de la file*/
    S_file* f = (S_file*)malloc(sizeof(S_file));
    if(f==NULL){
        printf("Erreur malloc S_file dans ppChemin\n");
        free(distance);
        free(visite);
        return NULL;
    }
    Init_file(f);
    enfile(f, u);
    visite[u] = 1;

    /*Création du tableau 'parent' pour la question 7.3*/
    int* parent = (int*)malloc(sizeof(int)*(g->nbsom));
    if(parent==NULL){
        printf("Erreur malloc tableau visite ppChemin\n");
        free(distance);
        free(visite);
        free(f);
        return NULL;
    }
    for(int i=0; i<(g->nbsom); i++){
        parent[i] = -1;
    }

    /*Boucle effectuant le parcours en largeur*/
    while(!(estFileVide(f))){
        int sd = defile(f);

        Cellule_arete* ca = g->T_som[sd]->L_voisin;
        while(ca){
            int sv;
            if(sd == ca->a->u){
                sv = ca->a->v;
            }else{
                sv = ca->a->u;
            }

            if(visite[sv]==0){
                visite[sv] = 1;
                distance[sv] = distance[sd] + 1;
                parent[sv] = sd;
                enfile(f, sv);
            }
            ca = ca->suiv;
        }
    }

    if(distance[v]==0){
        printf("Le sommet %d n'est pas accessible depuis le sommet %d\n", v, u);
        free(distance);
        free(visite);
        free(parent);
        free(f);
        return NULL;
    }
    
    int d = distance[v];
    free(distance);
    free(visite);

    /*On utilise la file d'avant qui est vide pour générer la chaine que nous retournerons*/
    /*On connait le dernier sommet de la chaine: v*/
    enfile(f, v);

    /*On retrace le chemin de v à u en remontant dans le tableau 'parent'*/
    int p;
    for(int i=0; i<d; i++){
        p = parent[(f->tete->val)];
        if(p == -1){
            printf("Erreur recherche parent ppChemin\n");
            free(f);
            return NULL;
        }

        /*On ajoute le parent de chaque sommet a la tete de la chaine*/
        Cellule_file *nouv=(Cellule_file *)malloc(sizeof(Cellule_file));
        nouv->val= p;
        nouv->suiv = f->tete;
        f->tete = nouv;
    }

    free(parent);
    return f;
}

/*Question 7.4*/
/*Fonctions creation et liberation de matrice*/
/*Creation matrice*/
int** creerMatrice(int nbLignes, int nbColonnes){
    int** matrice = (int**)malloc(sizeof(int*)*nbLignes);
    if(matrice==NULL){
        printf("Erreur malloc creerMatrice\n");
        return NULL;
    }

    for(int i=0; i<nbLignes; i++){
        matrice[i] = (int*)malloc(sizeof(int)*nbColonnes);
        if(matrice[i] == NULL){
            printf("Erreur malloc creerMatrice\n");
            /*Si malloc d'une ligne  echoue, nous devons libérer celles qui ont deja été créées*/
            for(int dejacree=0; dejacree<i; dejacree++){
                free(matrice[dejacree]);
            }
            free(matrice);
            return NULL;
        }

        /*Initialisation des valeurs du tableau à 0*/
        for(int j=0; j<nbColonnes; j++){
            matrice[i][j] = 0;
        }
    }

    return matrice;
}

/*Liberation matrice*/
void libererMatrice(int** matrice, int nbLignes){
    if(matrice == NULL){
        printf("Matrice deja libérée\n");
        return;
    }

    for(int i=0; i<nbLignes; i++){
        free(matrice[i]);
    }
    free(matrice);
}


int reorganiseReseau(Reseau* r){
    if(r==NULL){
        printf("Impossible de reorganiser Reseau qui n'existe pas\n");
        return -1;
    }

    Graphe *g = creerGraphe(r);

    int** matrice = creerMatrice(g->nbcommod, g->nbsom);

    /*Boucle passant par les commodités*/
    for(int i=0; i<(g->nbcommod); i++){
        S_file* chaine = ppChemin(g, g->T_commod[i].e1, g->T_commod[i].e2);

        if(chaine == NULL){
            printf("Commodite invalide\n");
            libererMatrice(matrice, g->nbcommod);
            libererGraphe(g);
            return -1;
        }

        /*On parcours la chaine pour mettre a jour la matrice*/
        int som;
        while(!(estFileVide(chaine))){
            som = defile(chaine);
            matrice[i][som] = 1;

            /*On controle a chaque fois le nombre de chaines passant par ce sommet*/
            int cpt = 0;
            for(int j=0; j<=i; j++){
                cpt += matrice[j][som];

                /*Cas ou plus de 3 chaines passent par un sommet*/
                if(cpt>(r->gamma)){
                    /*Nous vidons et libèrons la chaine, la matrice et le graphe*/
                    while(!(estFileVide(chaine))){
                        defile(chaine);
                    }
                    free(chaine);
                    libererMatrice(matrice, g->nbcommod);
                    libererGraphe(g);
                    return 0; /*Retourne faux*/
                }
            }
        }

        free(chaine);
    }

    /*Si nous sortons de la boucle, cela veut dire qu'il n'y a aucun sommet par lequel traversentplus de 3 chaines*/
    libererMatrice(matrice, g->nbcommod);
    libererGraphe(g);
    return 1; /*Retourne vrai*/
}