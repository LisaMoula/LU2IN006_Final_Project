
#include"Chaine.h"

//Question 1.2

//Fonctions de création des structs
//Création ensemble de chaines
Chaines* creer_chaines(int gamma, int nbChaines){
    // Alloue de la mémoire pour la structure Chaines.
    Chaines* C = (Chaines*)malloc(sizeof(Chaines));
    // Vérifie si l'allocation de mémoire a réussi.
    if(C == NULL){
        printf("Erreur création de Chaines (Gamma: %d ; NbChaines: %d)\n", gamma, nbChaines);
        return NULL;
    }

    // Initialise les champs de la structure Chaines.
    C->gamma = gamma;
    C->nbChaines = nbChaines;
    C->chaines = NULL;

    return C; // Renvoie l'ensemble de chaînes créé.
}


// Crée une nouvelle cellule de chaîne avec le numéro spécifié.
CellChaine* creer_cellChaine(int numero){
    // Alloue de la mémoire pour la structure CellChaine.
    CellChaine* c = (CellChaine*)malloc(sizeof(CellChaine));
    // Vérifie si l'allocation de mémoire a réussi.
    if(c == NULL){
        printf("Erreur création de la CellChaine numero %d\n", numero);
        return NULL;
    }

    // Initialise les champs de la structure CellChaine.
    c->numero = numero;
    c->points = NULL;
    c->suiv = NULL;

    return c; // Renvoie la cellule de chaîne créée.
}


// Crée une nouvelle cellule de point avec les coordonnées spécifiées.
CellPoint* creer_cellPoint(double x, double y){
    // Alloue de la mémoire pour la structure CellPoint.
    CellPoint* p = (CellPoint*)malloc(sizeof(CellPoint));
    // Vérifie si l'allocation de mémoire a réussi.
    if(p == NULL){
        printf("Erreur création du CellPoint %f %f\n", x, y);
        return NULL;
    }

    // Initialise les champs de la structure CellPoint.
    p->x = x;
    p->y = y;
    p->suiv = NULL;

    return p; // Renvoie la cellule de point créée.
}



//Fonctions liberation des structs
// Libère la mémoire allouée pour une cellule de point.
void liberer_cellPoint(CellPoint* p){
    // Vérifie si la cellule de point existe.
    if(p == NULL){
        printf("Point déjà libéré\n");
        return;
    }
    
    free(p); // Libère la mémoire de la cellule de point.
}

// Libère la mémoire allouée pour une cellule de chaîne et ses points associés.
void liberer_cellChaine(CellChaine* c){
    // Vérifie si la cellule de chaîne existe.
    if(c == NULL){
        printf("CellChaine déjà libéré\n");
        return;
    }

    // Libère la mémoire de chaque point dans la chaîne.
    CellPoint *prec, *cour = c->points;
    while(cour){
        prec = cour;
        cour = cour->suiv;
        liberer_cellPoint(prec);
    }
    
    free(c); // Libère la mémoire de la cellule de chaîne.
}


// Libère la mémoire allouée pour un ensemble de chaînes et toutes ses cellules de chaîne et points associés.
void liberer_chaines(Chaines* C){
    // Vérifie si l'ensemble de chaînes existe.
    if(C == NULL){
        printf("Ensemble de chaines déjà libéré\n");
        return;
    }

    // Libère la mémoire de chaque cellule de chaîne dans l'ensemble.
    CellChaine *prec, *cour = C->chaines;
    while(cour){
        prec = cour;
        cour = cour->suiv;
        liberer_cellChaine(prec);
    }
    
    free(C); // Libère la mémoire de l'ensemble de chaînes.
}



//Insertion en tete
// Insère une nouvelle chaîne au début de l'ensemble de chaînes.
void ins_en_tete_chaine(Chaines* C, int numero){
    // Vérifie si l'ensemble de chaînes existe.
    if(C == NULL){
        printf("Ensemble de chaines non créé, impossible d'insérer la chaine %d en tete\n", numero);
        return;
    }

    // Crée une nouvelle cellule de chaîne avec le numéro spécifié.
    CellChaine* c = creer_cellChaine(numero);
    // Vérifie si la création de la cellule de chaîne a réussi.
    if(c == NULL){
        printf("Impossible d'insérer la chaine en tete\n");
        return;
    }

    // Insère la nouvelle chaîne au début de l'ensemble de chaînes.
    c->suiv = C->chaines;
    C->chaines = c;
}


// Insère un nouveau point au début d'une chaîne.
void ins_en_tete_point(CellChaine* c, double x, double y){
    // Vérifie si la chaîne existe.
    if(c == NULL){
        printf("Chaine non créée, impossible d'insérer le point %f %f en tete\n", x, y);
        return;
    }

    // Crée une nouvelle cellule de point avec les coordonnées spécifiées.
    CellPoint* p = creer_cellPoint(x, y);
    // Vérifie si la création de la cellule de point a réussi.
    if(p == NULL){
        printf("Impossible d'insérer le point en tete\n");
        return;
    }

    // Insère le nouveau point au début de la chaîne.
    p->suiv = c->points;
    c->points = p;
}



// Lit un ensemble de chaînes à partir d'un fichier.
Chaines* lectureChaines(FILE* f){
    // Vérifie si le fichier est valide.
    if(f == NULL){
        printf("Fichier n'existe pas\n");
        return NULL;
    }
    
    int gamma, nbChaines;
    char buffer[256];
    // Lit le nombre de chaînes et le paramètre gamma depuis le fichier.
    fgets(buffer, 256, f);
    sscanf(buffer, "NbChain: %d", &nbChaines);
    fgets(buffer, 256, f);
    sscanf(buffer, "Gamma: %d", &gamma);
    fgets(buffer, 256, f); // Ignore la troisième ligne (saut de ligne)

    // Crée un nouvel ensemble de chaînes avec les valeurs lues depuis le fichier.
    Chaines* C = creer_chaines(gamma, nbChaines);
    // Vérifie si la création de l'ensemble de chaînes a réussi.
    if(C == NULL){
        printf("Ensemble de chaines non créées, impossible de lire les données\n");
        return NULL;
    }

    // Lit les chaînes et les points depuis le fichier.
    int numero, nbpoints;
    double x, y;
    char* donnee;
    for(int i = 0; i < nbChaines; i++){
        // Lit le numéro et le nombre de points de la chaîne.
        donnee = strtok(buffer, " ");
        sscanf(donnee, "%d", &numero);
        donnee = strtok(NULL, " ");
        sscanf(donnee, "%d", &nbpoints);
        donnee = strtok(NULL, " ");

        // Insère la chaîne dans l'ensemble de chaînes.
        ins_en_tete_chaine(C, numero);

        // Lit les coordonnées des points de la chaîne et les insère.
        for(int j = 0; j < nbpoints; j++){
            sscanf(donnee, "%lf", &x);
            donnee = strtok(NULL, " ");
            sscanf(donnee, "%lf", &y);
            donnee = strtok(NULL, " ");

            ins_en_tete_point(C->chaines, x, y);
        }

        fgets(buffer, 256, f); // Lit la ligne suivante du fichier.
    }

    return C; // Renvoie l'ensemble de chaînes lu depuis le fichier.
}



// Écrit un ensemble de chaînes dans un fichier.
void ecrireChaines(Chaines *C, FILE *f){
    // Vérifie si le fichier est valide.
    if(f == NULL){
        printf("Fichier n'existe pas\n");
        return;
    }

    // Vérifie si l'ensemble de chaînes est valide.
    if(C == NULL){
        printf("L'ensemble de chaines n'existe pas\n");
        return;
    }

    // Écrit les informations sur l'ensemble de chaînes dans le fichier.
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);

    // Écrit les chaînes dans le fichier.
    CellChaine* c = C->chaines;
    while(c != NULL){
        // Écrit le numéro de la chaîne dans le fichier.
        fprintf(f, "%d ", c->numero);

        int cpt = 0; // Initialise le compteur de points dans la chaîne.

        // Parcourt tous les points de la chaîne et les écrit dans le fichier.
        CellPoint* p = c->points;
        while(p != NULL){
            // Écrit les coordonnées du point dans le fichier.
            fprintf(f, "%.2f %.2f ", p->x, p->y);
            cpt++;
            p = p->suiv;
        }

        // Écrit le nombre de points dans la chaîne dans le fichier.
        fprintf(f, "%d\n", cpt);

        c = c->suiv; // Passe à la chaîne suivante.
    }
}




//Question 1.3
// Crée un fichier SVG en HTML à partir d'une structure Chaines.
void afficheChainesSVG(Chaines *C, char* nomInstance){

    /*int i;*/ /*Variable non untilisée*/

    // Déclare et initialise les variables pour les coordonnées maximales et minimales.
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx, precy;
    SVGwriter svg;
    ccour = C->chaines; // Initialise le pointeur de cellule de chaîne au début de l'ensemble de chaînes.

    // Parcourt toutes les chaînes pour trouver les coordonnées maximales et minimales.
    while (ccour != NULL){
        pcour = ccour->points; // Initialise le pointeur de cellule de point au premier point de la chaîne.

        // Parcourt tous les points dans la chaîne pour mettre à jour les coordonnées maximales et minimales.
        while (pcour != NULL){
            if (maxx < pcour->x) maxx = pcour->x;
            if (maxy < pcour->y) maxy = pcour->y;
            if (minx > pcour->x) minx = pcour->x;
            if (miny > pcour->y) miny = pcour->y;  
            pcour = pcour->suiv; // Passe au point suivant dans la chaîne.
        }
        ccour = ccour->suiv; // Passe à la chaîne suivante dans l'ensemble de chaînes.
    }

    // Initialise le fichier SVG avec les coordonnées maximales et minimales ajustées.
    SVGinit(&svg, nomInstance, 500, 500);

    ccour = C->chaines; // Réinitialise le pointeur de cellule de chaîne au début de l'ensemble de chaînes.
    // Parcourt toutes les chaînes pour dessiner chaque chaîne dans le fichier SVG.
    while (ccour != NULL){
        pcour = ccour->points; // Initialise le pointeur de cellule de point au premier point de la chaîne.

        // Dessine un point pour le premier point de la chaîne.
        SVGlineRandColor(&svg);
        SVGpoint(&svg, 500*(pcour->x-minx)/(maxx-minx), 500*(pcour->y-miny)/(maxy-miny)); 
        precx = pcour->x;
        precy = pcour->y;  
        pcour = pcour->suiv; // Passe au point suivant dans la chaîne.

        // Parcourt tous les points dans la chaîne pour dessiner les lignes et les points.
        while (pcour != NULL){
            // Dessine une ligne entre le point précédent et le point actuel.
            SVGline(&svg, 500*(precx-minx)/(maxx-minx), 500*(precy-miny)/(maxy-miny), 500*(pcour->x-minx)/(maxx-minx), 500*(pcour->y-miny)/(maxy-miny));
            // Dessine un point pour le point actuel.
            SVGpoint(&svg, 500*(pcour->x-minx)/(maxx-minx), 500*(pcour->y-miny)/(maxy-miny));
            precx = pcour->x;
            precy = pcour->y;    
            pcour = pcour->suiv; // Passe au point suivant dans la chaîne.
        }
        ccour = ccour->suiv; // Passe à la chaîne suivante dans l'ensemble de chaînes.
    }

    // Finalise le fichier SVG.
    SVGfinalize(&svg);
}


//Question 1.4
// Calcule la longueur d'une chaîne donnée.
double longueurChaine(CellChaine *c){
    // Vérifie si la chaîne existe.
    if(c == NULL){
        printf("Impossible de calculer la longueur de la chaîne qui n'existe pas\n");
        return -1;
    }

    CellPoint *a = c->points; // Initialise un pointeur de cellule de point au premier point de la chaîne.
    // Vérifie si la chaîne contient au moins un point.
    if(a == NULL){
        printf("La chaîne n'a pas de points\n");
        return 0;
    }

    CellPoint *b = a->suiv; // Initialise un pointeur de cellule de point au point suivant.
    // Vérifie si la chaîne contient plus d'un point.
    if(b == NULL){
        printf("La chaîne n'a qu'un seul point\n");
        return 0;
    }

    double xa, ya, xb, yb, longueur = 0; // Déclare des variables pour les coordonnées et la longueur.
    // Parcourt tous les points de la chaîne pour calculer la longueur totale.
    while(b){
        xa = a->x; // Récupère la coordonnée x du point a.
        ya = a->y; // Récupère la coordonnée y du point a.
        xb = b->x; // Récupère la coordonnée x du point b.
        yb = b->y; // Récupère la coordonnée y du point b.

        // Calcule la distance entre les points a et b et l'ajoute à la longueur totale.
        longueur += sqrt(pow((xb-xa),2) + pow((yb-ya),2));

        a = a->suiv; // Passe au point suivant dans la chaîne.
        b = b->suiv; // Passe au point suivant dans la chaîne.
    }

    return longueur; // Renvoie la longueur totale de la chaîne.
}



// Calcule la longueur totale de toutes les chaînes dans un ensemble de chaînes.
double longueurTotale(Chaines *C){
    // Vérifie si l'ensemble de chaînes existe.
    if(C == NULL){
        printf("Impossible de calculer la longueur d'un ensemble de chaînes qui n'existe pas\n");
        return -1;
    }

    double lc, longueur = 0; // Variable pour la longueur de la chaîne actuelle et une variable pour la longueur totale.
    CellChaine* c = C->chaines; // Pointeur de cellule de chaîne au début de l'ensemble de chaînes.
    // Parcourt toutes les chaînes pour calculer la longueur totale.
    while(c){
        // Calcule la longueur de la chaîne actuelle en appelant la fonction longueurChaine.
        lc = longueurChaine(c);

        // Vérifie si le calcul de la longueur de la chaîne actuelle a réussi.
        if(lc < 0){
            printf("Erreur calcul longueur totale\n");
            return -1;
        } else {
            longueur += lc; // Ajoute la longueur de la chaîne actuelle à la longueur totale.
        }

        c = c->suiv; // Passe à la chaîne suivante dans l'ensemble de chaînes.
    }

    return longueur; // Renvoie la longueur totale calculée.
}



// Compte le nombre total de points dans un ensemble de chaînes.
int comptePointsTotal(Chaines *C){
    // Vérifie si l'ensemble de chaînes existe.
    if(C == NULL){
        printf("Impossible de compter les points dans un ensemble de chaînes qui n'existe pas\n");
        return -1;
    }

    CellChaine* c = C->chaines; // Pointeur de cellule de chaîne au début de l'ensemble de chaînes.
    CellPoint* p;
    int cpt = 0; // Compteur de nombre de points.
    // Parcourt toutes les chaînes pour compter les points dans chaque chaîne.
    while(c){
        p = c->points; // Initialise le pointeur de cellule de point au début de la chaîne actuelle.

        // Parcourt tous les points dans la chaîne actuelle et incrémente le compteur.
        while(p){
            cpt++;
            p = p->suiv; // Passe au point suivant dans la chaîne.
        }

        c = c->suiv; // Passe à la chaîne suivante dans l'ensemble de chaînes.
    }

    return cpt; // Renvoie le nombre total de points.
}


/*Question 6.2*/
// Génère un ensemble de chaînes aléatoires.
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax){
    // Crée un ensemble de chaînes avec un gamma arbitraire de 3.
    Chaines* C = creer_chaines(3, nbChaines);
    if(C == NULL){
        printf("Chaines non créées pour generationAleatoire\n");
        return NULL;
    }

    srand(time(NULL)); // Initialise le générateur de nombres aléatoires avec une graine basée sur le temps.

    double x, y;

    // Parcourt toutes les chaînes.
    for(int i = 1; i <= nbChaines; i++){
        ins_en_tete_chaine(C, i); // Insère une nouvelle chaîne dans l'ensemble de chaînes.

        // Crée tous les points de la chaîne.
        for(int j = 0; j < nbPointsChaine; j++){
            // Définit les coordonnées du nouveau point de manière aléatoire dans la plage [0, xmax] pour x et [0, ymax] pour y.
            x = xmax * ((double)rand() / (double)RAND_MAX);
            y = ymax * ((double)rand() / (double)RAND_MAX);
            
            ins_en_tete_point(C->chaines, x, y); // Insère le point dans la chaîne.
        }
    }

    return C; // Renvoie l'ensemble de chaînes généré aléatoirement.
}


