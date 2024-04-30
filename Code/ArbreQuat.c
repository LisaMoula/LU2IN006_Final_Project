

#include"ArbreQuat.h"


/*Question 5.1*/
/*Récupération des coordonnees min et max d'une liste de chaines*/

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if(C==NULL){
        printf("Impossible de determiner coordonnees min et max de chaines qui n'existent pas\n");
        return;
    }

    CellChaine *c = C->chaines;
    if(c==NULL){
        printf("Il n'y a pas de chaines, il est impossible de determiner les coordonnees min et max\n");
        return;
    }
    CellPoint *p = c->points;
    if(p==NULL){
        printf("La chaine n'a pas de points, il est impossible de determiner les coordonnees min et max\n");
        return;
    }
    (*xmin) = p->x;
    (*ymin) = p->y;
    (*xmax) = p->x;
    (*ymax) = p->y;

    /*Boucle passant par toutes les chaines (Boucle for est plus sûre)*/

    for(int i=0; i<(C->nbChaines); i++){
        if(c==NULL){
            printf("Il y a moins de chaines que prévu, il est impossible de determiner les coordonnees min et max\n");
            return;
        }
        p = c->points;

        /*Boucle passant par tous les points de la chaine*/

        while(p){
            if(p->x < (*xmin)){ (*xmin) = p->x; }
            if(p->y < (*ymin)){ (*ymin) = p->y; }
            if(p->x > (*xmax)){ (*xmax) = p->x; }
            if(p->y > (*ymax)){ (*ymax) = p->y; }

            p = p->suiv;
        }

        c = c->suiv;
    }
}


/*Question 5.2*/
/*Creation Arbre quaternaire*/

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat* a = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    if(a==NULL){
        printf("Erreur malloc création ArbreQuat\n");
        return NULL;
    }

    a->xc = xc;
    a->yc = yc;
    a->coteX = coteX;
    a->coteY = coteY;
    a->noeud = NULL;
    a->so = NULL;
    a->se = NULL;
    a->no = NULL;
    a->ne = NULL;

    return a;
}


/*Liberation Arbre quaternaire*/

void liberer_ArbreQuat(ArbreQuat *a){
    if(a==NULL){
        printf("ArbreQuat est deja libéré\n");
        return;
    }

    a->noeud = NULL;

    if(a->so){ liberer_ArbreQuat(a->so); }
    if(a->se){ liberer_ArbreQuat(a->se); }
    if(a->no){ liberer_ArbreQuat(a->no); }
    if(a->ne){ liberer_ArbreQuat(a->ne); }

    free(a);
}


/*Determination direction dans laquelle inserer*/
ArbreQuat** direction(double x, double y, ArbreQuat *a){
    if(y >= a->yc){
        /*Nord*/
        if(x >= a->xc){
            /*Est*/
            return &(a->ne);
        }else{
            /*Ouest*/
            return &(a->no);
        }
    }else{
        /*Sud*/
        if(x >= a->xc){
            /*Est*/
            return &(a->se);
        }else{
            /*Ouest*/

            return &(a->so);
        }
    }
}


/*Insertion d'un noeud dans l'arbre*/

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    if(n==NULL){
        printf("Impossible d'insérer un noeud qui n'existe pas\n");
        return;
    }
    if(parent==NULL){
        printf("L'ArbreQuat parent n'existe pas, impossible d'insérer un noeud\n");
        return;
    }


    /*Cas où a est vide*/
    if((*a)==NULL){
        /*Determination longueur du nouvel arbre: Arbre fils occupe le quart de l'espace du parent*/
        double coteX = (parent->coteX)/2;
        double coteY = (parent->coteY)/2;

        /*Determination centre et position relative du nouvel arbre*/
        double xc, yc;
        if((n->y) >= (parent->yc)){
            /*Nord*/
            yc = (parent->yc) + (parent->coteY)/4;
        }else{
            /*Sud*/
            yc = (parent->yc) - (parent->coteY)/4;
        }
        if((n->x) >= (parent->xc)){
            /*Est*/
            xc = (parent->xc) + (parent->coteX)/4;
        }else{
            /*Ouest*/
            xc = (parent->xc) - (parent->coteX)/4;
        }

        /*Creation de l'arbre*/

        *a = creerArbreQuat(xc, yc, coteX, coteY);
        (*a)->noeud = n;

        return;
    }


    /*Cas où a est une feuille*/
    if((*a)->noeud != NULL){
        Noeud* deplace = (*a)->noeud; /*Noeud de la feuille qui doit etre deplacé*/
        (*a)->noeud = NULL; /*On enleve le noeud à a*/

        /*Insertion du noeud deplacé de la feuille*/
        insererNoeudArbre(deplace, direction(deplace->x, deplace->y, *a), *a);

        /*Insertion du nouveau noeud*/
        insererNoeudArbre(n, direction(n->x, n->y, *a), *a);
        return;
    }

    /*Cas où a est une cellule interne*/
    if((*a != NULL) && ((*a)->noeud == NULL)){
        /*Determination direction où sera inséré le noeud*/
        ArbreQuat **afils = direction(n->x, n->y, *a);
        /*Insertion du noeud dans cette arbre*/

        insererNoeudArbre(n, afils, *a);
        return;
    }
}


/*Insertion d'un noeud dans un Reseau (Necessaire ici pour ne pas réecrire le meme code dans 'rechercheCreeNoeudArbre')*/
Noeud* insererNoeudReseau(double x, double y, Reseau* R){
    /*Creation du nouveau noeud*/

    Noeud *n = creer_Noeud((R->nbNoeuds)+1, x, y);
    if(n==NULL){ 
    printf("Erreur création Noeud rechercheCreeNoeudArbre\n");
    return NULL;
    }


    /*Création CellNoeud du reseau*/

    CellNoeud *c = creer_CellNoeud(n);
    if(c==NULL){ 
        printf("Erreur création CellNoeud rechercheCreeNoeudListe\n");
        liberer_Noeud(n);
        return NULL;
    }

    /*Insertion noeud dans le reseau*/

    c->suiv = R->noeuds;
    R->noeuds = c;
    (R->nbNoeuds)++;

    return n;
}


/*Recherche / Création de noeud dans Reseau avec ArbreQuat*/

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    if(R==NULL){
        printf("Impossible de chercher noeud dans Reseau qui n'existe pas\n");
        return NULL;
    }
    if(parent==NULL){
        printf("Impossible de chercher noeud dans ArbreQuat qui n'existe pas\n");
        return NULL;
    }


    /*Recherche du noeud dans l'ArbreQuat*/
    /*Cas où a est vide*/
    if((*a)==NULL){
        Noeud* n = insererNoeudReseau(x, y, R);
        /*Insertion noeud dans arbre*/
        insererNoeudArbre(n, a, parent);
        return n;
    }
    /*Cas où a est une feuille*/

    if((*a)->noeud != NULL){
        if(((*a)->noeud->x == x) && ((*a)->noeud->y) == y){
            return (*a)->noeud;
        }else{
            Noeud* n = insererNoeudReseau(x, y, R);
            insererNoeudArbre(n, a, parent);
            return n;
        }
    }

    /*Cas où a est une cellule interne*/
    if((*a != NULL) && ((*a)->noeud == NULL)){
        return rechercheCreeNoeudArbre(R, direction(x, y, *a), *a, x, y);
    }
}

/*Reconstitution Reseau avec ArbreQuat a partir d'une Chaine*/

Reseau* reconstitueReseauArbre(Chaines* C){
    if(C==NULL){
        printf("Impossible de construire reseau a partir d'une chaine qui n'existe pas\n");
        return NULL;
    }


    /*Création Reseau*/

    Reseau* R = creer_Reseau(C->gamma);
    if(R==NULL){ 
        printf("Erreur création Réseau reconstitueReseauHachage\n");
        return NULL;
    }

    

    /*Determination coordonnees min et max de l'ArbreQuat*/

    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;
    double xc = xmax - coteX/2;
    double yc = ymax - coteY/2;


    /*Création ArbreQuat*/

    ArbreQuat *a = creerArbreQuat(xc, yc, coteX, coteY);
    if(a==NULL){
        printf("Erreur création ArbreQuat reconstitueReseauArbre\n");
        liberer_ArbreQuat(a);
        return NULL;
    }


    CellChaine *c = C->chaines; /*Chaines*/
    CellPoint *prec, *p; /*Points des chaines*/
    Noeud *extrA, *extrB, *cour; /*Noeuds: extremites des commodites et le noeud courant*/
    Noeud *voisin; /*La cellule du noeud ajouté précédemment*/
    ArbreQuat **afils; /*Adresse du fils de l'arbre a*/

    /*Boucle passant par toutes les chaines (Boucle for est plus fiable)*/

    for(int i=0; i<(C->nbChaines); i++){
        
        p = c->points;
        prec = NULL;
        voisin = NULL;


        /*Boucle passant par tous les points d'une chaine*/
        while(p){
            /*Determination dans quelle direction nous créons / cherchons le noeud*/
            afils = direction(p->x, p->y, a);

            /*Recherche ou ajout du noeud correspondant au point de la chaine*/

            cour = rechercheCreeNoeudArbre(R, afils, a, p->x, p->y);
            if(cour == NULL){
                printf("Erreur recherche noeud dans reconstitution du reseau\n");
                liberer_Reseau(R);
                liberer_ArbreQuat(a);
                return NULL;
            }


            /*Recuperation extremite A de la commodite*/

            if(prec == NULL){
                extrA = cour;
            }


            /*Mise a jour des voisins*/
            maj_voisins(R, voisin, cour);

            /*Le voisin du prochain noeud est ce noeud*/

            voisin = cour;
            prec = p;
            p = p->suiv;
        }


        /*Recuperation extremite B de la commodite*/
        extrB = cour;

        /*Creation et ajout de la commodite*/

        CellCommodite* C = creer_CellCommodite(extrA, extrB);
        if(C==NULL){
            printf("Erreur création commodite reconstitueReseauListe\n");
            liberer_Reseau(R);
            liberer_ArbreQuat(a);
            return NULL;
        }
        C->suiv = R->commodites;
        R->commodites = C;


        c = c->suiv; /*Passage a la chaine suivante*/
    }

    /*Liberation ArbreQuat*/

    liberer_ArbreQuat(a);

    return R;
}