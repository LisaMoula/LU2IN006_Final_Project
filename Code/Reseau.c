

#include"Reseau.h"

//Fonctions de création et libération
// Crée une nouvelle cellule de voisin contenant le noeud donné.
Noeud* creer_Noeud(int num, double x, double y){
    Noeud *n = (Noeud*)malloc(sizeof(Noeud));
    if(n==NULL){
        printf("Erreur malloc Noeud\n");
        return NULL;
    }

    n->num = num;
    n->x = x;
    n->y = y;
    n->voisins = NULL;

    return n;
}

//Liberation Noeud
// Libère la mémoire allouée pour un noeud et ses voisins
void liberer_Noeud(Noeud* n){
    if(n == NULL){
        printf("Noeud déjà libéré\n"); // Vérifie si le noeud est déjà libéré
        return;
    }

    CellNoeud *temp, *c = n->voisins; // Initialise des pointeurs pour parcourir la liste des voisins
    while(c){ // Parcourt la liste des voisins
        temp = c; // Stocke le voisin courant dans une variable temporaire
        c = c->suiv; // Passe au voisin suivant
        liberer_CellNoeud(temp); // Libère la mémoire allouée pour le voisin
    }

    free(n); // Libère la mémoire allouée pour le noeud lui-même
}



// Crée une nouvelle cellule de noeud et l'initialise avec le noeud spécifié
CellNoeud* creer_CellNoeud(Noeud* n){
    CellNoeud *c = (CellNoeud*)malloc(sizeof(CellNoeud)); // Alloue de la mémoire pour la cellule de noeud
    if(c == NULL){
        printf("Erreur malloc CellNoeud\n"); // Vérifie si l'allocation de mémoire a échoué
        return NULL;
    }

    c->nd = n; // Initialise le pointeur vers le noeud
    c->suiv = NULL; // Initialise le pointeur vers le voisin suivant à NULL

    return c; // Retourne la nouvelle cellule de noeud créée
}


// Libère la mémoire allouée pour une cellule de noeud
void liberer_CellNoeud(CellNoeud *c){
    if(c == NULL){
        printf("CellNoeud déjà libéré\n"); // Vérifie si la cellule est déjà libérée
        return;
    }

    c->nd = NULL; // Définit le noeud pointé par la cellule à NULL pour éviter les accès accidentels
    free(c); // Libère la mémoire allouée pour la cellule
}



// Crée une cellule de commodité avec les extrémités données
CellCommodite* creer_CellCommodite(Noeud* extrA, Noeud* extrB){
    // Vérifie si les extrémités sont valides
    if(extrA == NULL || extrB == NULL){
        printf("Erreur création CellCommodite: extremité n'existe pas\n");
        return NULL;
    }

    // Alloue de la mémoire pour la cellule de commodité
    CellCommodite* C = (CellCommodite*)malloc(sizeof(CellCommodite));
    if(C == NULL){
        printf("Erreur malloc CellCommodite\n"); // Vérifie si l'allocation a échoué
        return NULL;
    }

    // Initialise les extrémités de la commodité avec les valeurs données
    C->extrA = extrA;
    C->extrB = extrB;

    return C; // Retourne la cellule de commodité nouvellement créée
}


// Libère la mémoire allouée pour une cellule de commodité
void liberer_CellCommodite(CellCommodite *C){
    // Vérifie si la cellule de commodité existe
    if(C == NULL){
        printf("Commodite déjà libérée\n"); // Indique que la commodité est déjà libérée
        return;
    }

    // Définit les extrémités de la commodité comme nulles
    C->extrA = NULL;
    C->extrB = NULL;

    free(C); // Libère la mémoire de la cellule de commodité
}



// Crée une nouvelle instance de réseau
Reseau* creer_Reseau(int gamma){
    // Alloue de la mémoire pour la nouvelle instance de réseau
    Reseau* r = (Reseau*)malloc(sizeof(Reseau));
    // Vérifie si l'allocation de mémoire a réussi
    if(r == NULL){
        printf("Erreur malloc Reseau\n"); // Affiche un message d'erreur si l'allocation a échoué
        return NULL; // Retourne NULL pour indiquer une erreur
    }

    // Initialise les attributs du réseau
    r->nbNoeuds = 0;
    r->gamma = gamma;
    r->noeuds = NULL;
    r->commodites = NULL;

    return r; // Retourne la nouvelle instance de réseau
}


// Libère la mémoire allouée pour une instance de réseau
void liberer_Reseau(Reseau *R){
    // Vérifie si le réseau existe
    if(R == NULL){
        printf("Reseau deja libéré\n"); // Affiche un message indiquant que le réseau est déjà libéré
        return; // Quitte la fonction
    }

    // Libération des noeuds
    CellNoeud *tempc, *c = R->noeuds;
    while(c){
        tempc = c; // Stocke le pointeur actuel dans une variable temporaire
        c = c->suiv; // Avance au prochain noeud dans la liste
        liberer_Noeud(tempc->nd); // Libère le noeud courant
        liberer_CellNoeud(tempc); // Libère la cellule de noeud courante
    }

    // Libération des commodités
    CellCommodite *tempC, *C = R->commodites;
    while(C){
        tempC = C; // Stocke le pointeur actuel dans une variable temporaire
        C = C->suiv; // Avance au prochain élément dans la liste
        liberer_CellCommodite(tempC); // Libère la cellule de commodité courante
    }

    free(R); // Libère la mémoire allouée pour l'instance de réseau
}




//Question 2.1
// Recherche un nœud dans le réseau et le crée s'il n'existe pas
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    // Vérifie si le réseau est valide
    if(R == NULL){
        printf("Impossible de chercher noeud: Reseau non créé\n"); // Affiche un message d'erreur
        return NULL; // Renvoie NULL car le réseau n'est pas valide
    }

    // Recherche du nœud dans le réseau
    CellNoeud *c = R->noeuds, *prec = NULL;
    for(int i = 0; (i < R->nbNoeuds) && (c->nd->x <= x); i++){
        // Vérifie si les coordonnées x correspondent
        if(c->nd->x == x){
            // Vérifie si les coordonnées y correspondent
            if(c->nd->y == y){
                return c->nd; // Renvoie le nœud s'il est trouvé dans le réseau
            }else if(c->nd->y > y){
                break; // Sort de la boucle car la coordonnée y dépasse la valeur recherchée
            }
        }

        prec = c; // Stocke la cellule actuelle comme précédente
        c = c->suiv; // Avance à la cellule suivante
    }

    // Si le nœud n'est pas trouvé dans le réseau, il doit être créé puis ajouté
    Noeud *n = creer_Noeud((R->nbNoeuds) + 1, x, y); // Crée un nouveau nœud avec l'index incrémenté
    if(n == NULL){ 
        printf("Erreur création Noeud rechercheCreeNoeudListe\n"); // Affiche un message d'erreur
        return NULL; // Renvoie NULL en cas d'erreur de création du nœud
    }

    CellNoeud* cn = creer_CellNoeud(n); // Crée une nouvelle cellule pour le nœud
    if(cn == NULL){ 
        printf("Erreur création CellNoeud rechercheCreeNoeudListe\n"); // Affiche un message d'erreur
        liberer_Noeud(n); // Libère la mémoire allouée pour le nœud
        return NULL; // Renvoie NULL en cas d'erreur de création de la cellule
    }

    // Ajoute le nœud à la liste de manière à garder la liste des nœuds triée
    if(prec == NULL){
        cn->suiv = R->noeuds;
        R->noeuds = cn;
    }else{
        cn->suiv = c;
        prec->suiv = cn;
    }

    (R->nbNoeuds)++; // Incrémente le nombre total de nœuds dans le réseau

    return n; // Renvoie le nœud créé ou trouvé dans le réseau
}


//Question 2.2
// Met à jour la liste des voisins pour deux nœuds dans le réseau
void maj_voisins(Reseau *R, Noeud *v, Noeud *n){
    // Vérifie si le nœud n est valide
    if(n == NULL){
        printf("Impossible de mettre à jour un noeud qui n'existe pas\n"); // Affiche un message d'erreur
        return; // Sort de la fonction
    }

    // Vérifie si le nœud v est NULL (cas où il n'y a pas de voisin à ajouter)
    if(v == NULL){
        return; // Sort de la fonction car aucun voisin à ajouter
    }

    // Vérifie si le voisin v est déjà ajouté à la liste des voisins du nœud n
    CellNoeud *dejala = n->voisins;
    while(dejala){
        if(dejala->nd->num == v->num){ // Compare les numéros de nœuds pour vérifier si le voisin est déjà ajouté
            return; // Sort de la fonction car le voisin est déjà ajouté
        }
        dejala = dejala->suiv; // Passe au voisin suivant dans la liste
    }

    // Ajoute le nœud voisin v à la liste des voisins du nœud n
    CellNoeud *voisin = creer_CellNoeud(v); // Crée une nouvelle cellule pour le voisin v
    voisin->suiv = n->voisins; // Ajoute le voisin à la tête de la liste des voisins de n
    n->voisins = voisin; // Met à jour la tête de la liste des voisins de n avec le nouveau voisin

    // Mise à jour des voisins du voisin v avec le nœud n
    CellNoeud *vois_du_vois = creer_CellNoeud(n); // Crée une nouvelle cellule pour le nœud n
    vois_du_vois->suiv = v->voisins; // Ajoute le nœud n à la tête de la liste des voisins de v
    v->voisins = vois_du_vois; // Met à jour la tête de la liste des voisins de v avec le nouveau nœud
}


// Reconstitue un réseau à partir d'une structure Chaines
Reseau* reconstitueReseauListe(Chaines *C){
    // Vérifie si la structure Chaines est valide
    if(C == NULL){
        printf("Impossible de construire un réseau à partir d'une chaîne inexistante\n");
        return NULL;
    }

    // Crée un nouveau réseau avec le paramètre gamma de la structure Chaines
    Reseau* R = creer_Reseau(C->gamma);
    if(R == NULL){ 
        printf("Erreur lors de la création du réseau dans reconstitueReseauListe\n");
        return NULL;
    }

    // Parcours de toutes les chaines dans la structure Chaines
    CellChaine *c = C->chaines; // Pointeur vers la première chaîne
    CellPoint *prec, *p; // Points des chaines
    Noeud *extrA, *extrB, *cour; // Extrémités des commodités et nœud courant
    Noeud *voisin = NULL; // Nœud précédemment ajouté

    for(int i = 0; i < C->nbChaines; i++){ // Boucle pour chaque chaîne
        p = c->points; // Pointeur vers le premier point de la chaîne
        prec = NULL; // Pointeur vers le point précédent (initialisé à NULL)
        voisin = NULL; // Réinitialise le voisin précédent à NULL
        while(p){ // Parcours de tous les points de la chaîne
            // Recherche ou création du nœud correspondant au point de la chaîne
            cour = rechercheCreeNoeudListe(R, p->x, p->y);
            if(cour == NULL){
                printf("Erreur lors de la recherche du nœud dans la reconstitution du réseau\n");
                liberer_Reseau(R);
                return NULL;
            }

            // Récupération de l'extrémité A de la commodité
            if(prec == NULL){
                extrA = cour;
            }

            // Mise à jour des voisins
            maj_voisins(R, voisin, cour);

            // Le voisin du prochain nœud est le nœud courant
            voisin = cour;
            prec = p;
            p = p->suiv; // Passage au point suivant dans la chaîne
        }

        // Récupération de l'extrémité B de la commodité
        extrB = cour;

        // Création et ajout de la commodité
        CellCommodite* C = creer_CellCommodite(extrA, extrB);
        if(C == NULL){
            printf("Erreur lors de la création de la commodité dans reconstitueReseauListe\n");
            liberer_Reseau(R);
            return NULL;
        }
        C->suiv = R->commodites; // Ajout de la commodité à la tête de la liste des commodités
        R->commodites = C;

        c = c->suiv; // Passage à la chaîne suivante dans la structure Chaines
    }

    return R; // Retourne le réseau reconstitué
}


// Compte le nombre de liaisons dans le réseau
int nbLiaisons(Reseau *R){
    // Vérifie si le réseau est valide
    if(R == NULL){
        printf("Impossible de compter le nombre de liaisons d'un réseau qui n'existe pas\n");
        return -1;
    }

    int cpt = 0; // Initialisation du compteur de liaisons
    Noeud* dejaVu[R->nbNoeuds]; // Tableau pour stocker les nœuds déjà vus
    int vu = 0; // Variable booléenne pour vérifier si un nœud a été vu ou non

    CellNoeud *vois, *c = R->noeuds; // Pointeur vers la liste des nœuds dans le réseau
    // Parcours de tous les nœuds du réseau
    for(int i = 0; i < R->nbNoeuds; i++){
        vois = c->nd->voisins; // Pointeur vers la liste des voisins du nœud courant

        // Parcours de tous les voisins du nœud courant
        while(vois){
            vu = 0; // Réinitialisation de la variable booléenne

            // Parcours des nœuds déjà vus
            for(int j = 0; j < i; j++){
                if(dejaVu[j] == vois->nd){
                    vu = 1; // Le nœud a été vu
                    break;
                }
            }

            // Si le nœud n'a pas été vu, on incrémente le compteur de liaisons
            if(!(vu)){
                cpt++;
            }

            vois = vois->suiv; // Passage au voisin suivant du nœud courant
        }

        dejaVu[i] = c->nd; // Ajout du nœud courant dans le tableau des nœuds déjà vus
        c = c->suiv; // Passage au nœud suivant dans la liste des nœuds
    }

    return cpt; // Retourne le nombre de liaisons
}


// Compte le nombre de commodités dans le réseau
int nbCommodites(Reseau *R){
    // Vérifie si le réseau est valide
    if(R == NULL){
        printf("Impossible de compter le nombre de commodités d'un réseau qui n'existe pas\n");
        return -1;
    }

    int cpt = 0; // Initialisation du compteur de commodités
    CellCommodite *C = R->commodites; // Pointeur vers la liste des commodités dans le réseau

    // Parcours de toutes les commodités dans le réseau
    while(C){
        cpt++; // Incrémentation du compteur
        C = C->suiv; // Passage à la commodité suivante
    }

    return cpt; // Retourne le nombre de commodités
}



// Écriture du réseau dans un fichier
void ecrireReseau(Reseau *R, FILE *f){
    // Vérifie si le réseau et le fichier sont valides
    if(R == NULL){
        printf("Impossible d'écrire un réseau qui n'existe pas\n");
        return;
    }
    if(f == NULL){
        printf("Impossible d'écrire dans un fichier qui ne s'est pas ouvert\n");
        return;
    }

    // Écriture des informations générales du réseau dans le fichier
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "nbCommodites: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n\n", R->gamma);

    // Écriture des noeuds et des liaisons du réseau dans le fichier
    Noeud* dejaVu[R->nbNoeuds]; // Tableau pour stocker les noeuds déjà vus
    int vu = 0; // Variable booléenne pour vérifier si un noeud a été vu ou non

    CellNoeud *c = R->noeuds, *vois;
    char liaisbuffer[256]; // Buffer pour stocker l'écriture des liaisons
    char buffer[256] = ""; // Buffer initial sur lequel les liaisons seront concaténées

    // Parcours de tous les noeuds du réseau
    for(int i = 0; i < R->nbNoeuds; i++){
        // Écriture du noeud dans le fichier
        fprintf(f, "v %d %lf %lf\n", c->nd->num, c->nd->x, c->nd->y);

        // Parcours de tous les voisins du noeud
        vois = c->nd->voisins;
        while(vois){
            vu = 0;
            // Vérifie si le voisin a déjà été vu
            for(int j = 0; j < i; j++){
                if(dejaVu[j] == vois->nd){
                    vu = 1;
                    break;
                }
            }

            // Si le voisin n'a pas été vu, écrit la liaison dans le buffer
            if(!(vu)){
                sprintf(liaisbuffer, "l %d %d\n", vois->nd->num, c->nd->num);
                strcat(buffer, liaisbuffer);
            }

            vois = vois->suiv;
        }

        dejaVu[i] = c->nd;
        c = c->suiv;
    }
    fprintf(f, "\n%s\n", buffer); // Écriture des liaisons dans le fichier

    // Écriture des commodités dans le fichier
    CellCommodite *C = R->commodites;
    while(C){
        fprintf(f, "k %d %d\n", C->extrA->num, C->extrB->num);
        C = C->suiv;
    }
}


// Affichage du réseau en SVG à partir du réseau
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN, *courv;
    SVGwriter svg;
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;

    // Recherche des coordonnées maximales et minimales pour le redimensionnement
    courN = R->noeuds;
    while (courN != NULL){
        if (maxx < courN->nd->x) maxx = courN->nd->x;
        if (maxy < courN->nd->y) maxy = courN->nd->y;
        if (minx > courN->nd->x) minx = courN->nd->x;
        if (miny > courN->nd->y) miny = courN->nd->y;
        courN = courN->suiv;
    }

    // Initialisation du fichier SVG
    SVGinit(&svg, nomInstance, 500, 500);

    // Dessin des points et des liaisons du réseau
    courN = R->noeuds;
    while (courN != NULL){
        SVGpoint(&svg, 500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy - miny));
        courv = courN->nd->voisins;
        while (courv != NULL){
            // Dessine seulement une liaison pour chaque paire de noeuds pour éviter la redondance
            if (courv->nd->num < courN->nd->num)
                SVGline(&svg, 500 * (courv->nd->x - minx) / (maxx - minx), 500 * (courv->nd->y - miny) / (maxy - miny),
                        500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy - miny));
            courv = courv->suiv;
        }
        courN = courN->suiv;
    }

    // Finalisation du fichier SVG
    SVGfinalize(&svg);
}
