

#include"ArbreQuat.h"


/*Question 5.1*/
/*Récupération des coordonnees min et max d'une liste de chaines*/

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    // Vérifie si la structure de données des chaînes existe
    if(C == NULL){
        printf("Impossible de déterminer les coordonnées min et max des chaînes qui n'existent pas\n");
        return;
    }

    // Initialise les pointeurs pour parcourir les chaînes et les points
    CellChaine *c = C->chaines;
    // Vérifie s'il y a des chaînes dans la structure
    if(c == NULL){
        printf("Il n'y a pas de chaînes, il est impossible de déterminer les coordonnées min et max\n");
        return;
    }
    CellPoint *p = c->points;
    // Vérifie s'il y a des points dans la première chaîne
    if(p == NULL){
        printf("La chaîne n'a pas de points, il est impossible de déterminer les coordonnées min et max\n");
        return;
    }

    // Initialise les valeurs min et max avec les coordonnées du premier point
    (*xmin) = p->x;
    (*ymin) = p->y;
    (*xmax) = p->x;
    (*ymax) = p->y;

    // Boucle parcourant toutes les chaînes
    for(int i = 0; i < C->nbChaines; i++){
        // Vérifie s'il y a une chaîne pour chaque itération
        if(c == NULL){
            printf("Il y a moins de chaînes que prévu, il est impossible de déterminer les coordonnées min et max\n");
            return;
        }
        p = c->points;

        // Boucle parcourant tous les points de la chaîne
        while(p){
            // Met à jour les valeurs min et max si nécessaire
            if(p->x < (*xmin)){ (*xmin) = p->x; }
            if(p->y < (*ymin)){ (*ymin) = p->y; }
            if(p->x > (*xmax)){ (*xmax) = p->x; }
            if(p->y > (*ymax)){ (*ymax) = p->y; }

            p = p->suiv;
        }

        c = c->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    // Alloue de la mémoire pour l'arbre
    ArbreQuat* a = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    // Vérifie si l'allocation mémoire a réussi
    if(a == NULL){
        printf("Erreur lors de l'allocation mémoire pour la création de l'arbre quaternaire\n");
        return NULL;
    }

    // Initialise les valeurs de l'arbre avec les paramètres fournis
    a->xc = xc;     // Coordonnée x du centre de l'arbre
    a->yc = yc;     // Coordonnée y du centre de l'arbre
    a->coteX = coteX;   // Longueur du côté en direction de l'axe des abscisses
    a->coteY = coteY;   // Longueur du côté en direction de l'axe des ordonnées
    a->noeud = NULL;    // Noeud stocké dans l'arbre (NULL pour le moment)
    a->so = NULL;       // Sous-arbre sud-ouest
    a->se = NULL;       // Sous-arbre sud-est
    a->no = NULL;       // Sous-arbre nord-ouest
    a->ne = NULL;       // Sous-arbre nord-est

    return a;
}



/*Liberation Arbre quaternaire*/

void liberer_ArbreQuat(ArbreQuat *a){
    // Vérifie si l'arbre est déjà libéré
    if(a == NULL){
        printf("L'arbre quaternaire est déjà libéré\n");
        return;
    }

    // Libère récursivement les sous-arbres
    if(a->so){ liberer_ArbreQuat(a->so); }
    if(a->se){ liberer_ArbreQuat(a->se); }
    if(a->no){ liberer_ArbreQuat(a->no); }
    if(a->ne){ liberer_ArbreQuat(a->ne); }

    // Libère l'arbre actuel
    free(a);
}

/* Détermine la direction dans laquelle insérer un noeud */
ArbreQuat** direction(double x, double y, ArbreQuat *a){
    if(y >= a->yc){
        // Nord
        if(x >= a->xc){
            // Est
            return &(a->ne);
        }else{
            // Ouest
            return &(a->no);
        }
    }else{
        // Sud
        if(x >= a->xc){
            // Est
            return &(a->se);
        }else{
            // Ouest
            return &(a->so);
        }
    }
}

/* Insère un noeud dans l'arbre quaternaire */
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    // Vérifie si le noeud à insérer et l'arbre parent existent
    if(n == NULL){
        printf("Impossible d'insérer un noeud qui n'existe pas\n");
        return;
    }
    if(parent == NULL){
        printf("L'arbre quaternaire parent n'existe pas, impossible d'insérer un noeud\n");
        return;
    }

    /* Cas où a est vide */
    if((*a) == NULL){
        // Détermine la taille du nouvel arbre (un quart de l'espace du parent)
        double coteX = (parent->coteX) / 2;
        double coteY = (parent->coteY) / 2;

        // Détermine le centre et la position relative du nouvel arbre
        double xc, yc;
        if((n->y) >= (parent->yc)){
            yc = (parent->yc) + (parent->coteY) / 4; // Nord
        }else{
            yc = (parent->yc) - (parent->coteY) / 4; // Sud
        }
        if((n->x) >= (parent->xc)){
            xc = (parent->xc) + (parent->coteX) / 4; // Est
        }else{
            xc = (parent->xc) - (parent->coteX) / 4; // Ouest
        }

        // Crée le nouvel arbre
        *a = creerArbreQuat(xc, yc, coteX, coteY);
        (*a)->noeud = n;

        return;
    }

    /* Cas où a est une feuille */
    if((*a)->noeud != NULL){
        Noeud* deplace = (*a)->noeud; // Noeud de la feuille qui doit être déplacé
        (*a)->noeud = NULL; // Supprime le noeud de la feuille

        // Insère le noeud déplacé de la feuille
        insererNoeudArbre(deplace, direction(deplace->x, deplace->y, *a), *a);

        // Insère le nouveau noeud
        insererNoeudArbre(n, direction(n->x, n->y, *a), *a);
        return;
    }

    /* Cas où a est une cellule interne */
    if((*a != NULL) && ((*a)->noeud == NULL)){
        // Détermine la direction où sera inséré le noeud
        ArbreQuat **afils = direction(n->x, n->y, *a);
        // Insère le noeud dans cet arbre
        insererNoeudArbre(n, afils, *a);
        return;
    }
}



/*Insertion d'un noeud dans un Reseau (Necessaire ici pour ne pas réecrire le meme code dans 'rechercheCreeNoeudArbre')*/
Noeud* insererNoeudReseau(double x, double y, Reseau* R){
    // Crée un nouveau noeud
    Noeud *n = creer_Noeud((R->nbNoeuds) + 1, x, y);
    if(n == NULL){ 
        printf("Erreur lors de la création du noeud dans insererNoeudReseau\n");
        return NULL;
    }

    // Crée une cellule de noeud pour le réseau
    CellNoeud *c = creer_CellNoeud(n);
    if(c == NULL){ 
        printf("Erreur lors de la création de la cellule de noeud dans insererNoeudReseau\n");
        liberer_Noeud(n);
        return NULL;
    }

    // Insère le noeud dans le réseau
    c->suiv = R->noeuds;
    R->noeuds = c;
    (R->nbNoeuds)++;

    return n;
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    // Vérifie si le réseau et l'arbre parent existent
    if(R == NULL){
        printf("Impossible de chercher un noeud dans un réseau qui n'existe pas\n");
        return NULL;
    }
    if(parent == NULL){
        printf("Impossible de chercher un noeud dans un arbre quaternaire qui n'existe pas\n");
        return NULL;
    }

    // Recherche du noeud dans l'arbre quaternaire
    if((*a) == NULL){
        // Cas où l'arbre est vide, insère un nouveau noeud dans le réseau et dans l'arbre
        Noeud* n = insererNoeudReseau(x, y, R);
        insererNoeudArbre(n, a, parent);
        return n;
    }

    // Cas où l'arbre est une feuille
    if((*a)->noeud != NULL){
        // Vérifie si le noeud existant a les mêmes coordonnées que celles spécifiées
        if(((*a)->noeud->x == x) && ((*a)->noeud->y == y)){
            return (*a)->noeud; // Le noeud existe déjà, retourne-le
        }else{
            // Sinon, insère un nouveau noeud dans le réseau et dans l'arbre
            Noeud* n = insererNoeudReseau(x, y, R);
            insererNoeudArbre(n, a, parent);
            return n;
        }
    }

    // Cas où l'arbre est une cellule interne, poursuit la recherche dans la bonne direction
    if((*a != NULL) && ((*a)->noeud == NULL)){
        return rechercheCreeNoeudArbre(R, direction(x, y, *a), *a, x, y);
    }
}


/*Reconstitution Reseau avec ArbreQuat a partir d'une Chaine*/

Reseau* reconstitueReseauArbre(Chaines* C){
    // Vérifie si la chaîne existe
    if(C == NULL){
        printf("Impossible de construire un réseau à partir d'une chaîne qui n'existe pas\n");
        return NULL;
    }

    // Crée un réseau
    Reseau* R = creer_Reseau(C->gamma);
    if(R == NULL){ 
        printf("Erreur lors de la création du réseau dans reconstitueReseauArbre\n");
        return NULL;
    }

    // Détermine les coordonnées min et max de l'arbre quaternaire
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;
    double xc = xmax - coteX/2;
    double yc = ymax - coteY/2;

    // Crée un arbre quaternaire
    ArbreQuat *a = creerArbreQuat(xc, yc, coteX, coteY);
    if(a == NULL){
        printf("Erreur lors de la création de l'arbre quaternaire dans reconstitueReseauArbre\n");
        liberer_Reseau(R);
        return NULL;
    }

    CellChaine *c = C->chaines; // Chaines
    CellPoint *prec, *p; // Points des chaines
    Noeud *extrA, *extrB, *cour; // Noeuds: extrémités des commodités et le noeud courant
    Noeud *voisin; // Le noeud ajouté précédemment
    ArbreQuat **afils; // Adresse du fils de l'arbre a

    // Boucle passant par toutes les chaines
    for(int i = 0; i < C->nbChaines; i++){
        p = c->points;
        prec = NULL;
        voisin = NULL;

        // Boucle passant par tous les points d'une chaine
        while(p){
            // Détermine dans quelle direction nous créons / cherchons le noeud
            afils = direction(p->x, p->y, a);

            // Recherche ou ajout du noeud correspondant au point de la chaine
            cour = rechercheCreeNoeudArbre(R, afils, a, p->x, p->y);
            if(cour == NULL){
                printf("Erreur lors de la recherche du noeud dans la reconstitution du réseau\n");
                liberer_Reseau(R);
                liberer_ArbreQuat(a);
                return NULL;
            }

            // Récupération de l'extrémité A de la commodité
            if(prec == NULL){
                extrA = cour;
            }

            // Mise à jour des voisins
            maj_voisins(R, voisin, cour);

            // Le voisin du prochain noeud est ce noeud
            voisin = cour;
            prec = p;
            p = p->suiv;
        }

        // Récupération de l'extrémité B de la commodité
        extrB = cour;

        // Crée et ajoute la commodité
        CellCommodite* C = creer_CellCommodite(extrA, extrB);
        if(C == NULL){
            printf("Erreur lors de la création de la commodité dans reconstitueReseauArbre\n");
            liberer_Reseau(R);
            liberer_ArbreQuat(a);
            return NULL;
        }
        C->suiv = R->commodites;
        R->commodites = C;

        c = c->suiv; // Passage à la chaine suivante
    }

    // Libère l'arbre quaternaire
    liberer_ArbreQuat(a);

    return R;
}
