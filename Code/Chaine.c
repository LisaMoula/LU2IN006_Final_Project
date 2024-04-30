
#include"Chaine.h"

//Question 1.2

//Fonctions de création des structs
//Création ensemble de chaines
Chaines* creer_chaines(int gamma, int nbChaines){
    Chaines* C = (Chaines*)malloc(sizeof(Chaines));
    if(C == NULL){
        printf("Erreur création de Chaines (Gamma: %d ; NbChaines: %d)\n", gamma, nbChaines);
        return NULL;
    }

    C->gamma = gamma;
    C->nbChaines = nbChaines;
    C->chaines = NULL;

    return C;
}

//Création liste de chaines
CellChaine* creer_cellChaine(int numero){
    CellChaine* c = (CellChaine*)malloc(sizeof(CellChaine));
    if(c == NULL){
        printf("Erreur création de la CellChaine numero %d\n", numero);
        return NULL;
    }

    c->numero = numero;
    c->points = NULL;
    c->suiv = NULL;

    return c;
}

//Création point
CellPoint* creer_cellPoint(double x, double y){
    CellPoint* p = (CellPoint*)malloc(sizeof(CellPoint));
    if(p == NULL){
        printf("Erreur création du CellPoint %f %f\n", x, y);
        return NULL;
    }

    p->x = x;
    p->y = y;
    p->suiv = NULL;

    return p;
}


//Fonctions liberation des structs
//Liberation point (ne sert que pour la verification si p est NULL)
void liberer_cellPoint(CellPoint* p){
    if(p == NULL){
        printf("Point déjà libéré\n");
        return;
    }
    
    free(p);
}

//Liberation liste de chaines
void liberer_cellChaine(CellChaine* c){
    if(c == NULL){
        printf("CellChaine déjà libéré\n");
        return;
    }

    //Liberation de la liste de points
    CellPoint *prec, *cour = c->points;
    while(cour){
        prec = cour;
        cour = cour->suiv;
        liberer_cellPoint(prec);
    }
    
    free(c);
}

//Liberation ensemble de chaines
void liberer_chaines(Chaines* C){
    if(C == NULL){
        printf("Ensemble de chaines déjà libéré\n");
        return;
    }

    //Libération des cellules de chaine
    CellChaine *prec, *cour = C->chaines;
    while(cour){
        prec = cour;
        cour = cour->suiv;
        liberer_cellChaine(prec);
    }
    
    free(C);
}


//Insertion en tete
//Insertion d'une chaine dans un ensemble de chaines
void ins_en_tete_chaine(Chaines* C, int numero){
    if(C==NULL){
        printf("Ensemble de chaines non créée, impossible d'inserer la chaine %d en tete\n", numero);
        return;
    }

    CellChaine* c = creer_cellChaine(numero);
    if(c==NULL){
        printf("Impossible d'insérer la chaine en tete\n");

        return;

    }

    c->suiv = C->chaines;
    C->chaines = c;


    return;

}

//Insertion d'un point dans une chaine
void ins_en_tete_point(CellChaine* c, double x, double y){
    if(c==NULL){
        printf("Chaine non créée, impossible d'inserer le point %f %f en tete\n", x, y);
        return;
    }

    CellPoint* p = creer_cellPoint(x, y);
    if(p==NULL){
        printf("Impossible d'insérer le point en tete\n");
    }

    p->suiv = c->points;
    c->points = p;
}


//Creation d'un ensemble de chaines par lecture d'un fichier
Chaines* lectureChaines(FILE* f){
    if(f==NULL){
        printf("Fichier n'existe pas\n"); // Vérifie si le fichier est valide
        return NULL;
    }
    
    // Lecture de Gamma et NbChaines depuis le fichier
    int gamma, nbChaines;
    char buffer[256];
    fgets(buffer, 256, f); // Lecture de la première ligne
    sscanf(buffer, "NbChain: %d", &nbChaines); // Extrait le nombre de chaînes depuis la première ligne
    fgets(buffer, 256, f); // Lecture de la deuxième ligne
    sscanf(buffer, "Gamma: %d", &gamma); // Extrait la valeur de gamma depuis la deuxième ligne
    fgets(buffer, 256, f); // Lecture de la troisième ligne (saut de ligne)

    // Création de l'ensemble des chaînes
    Chaines* C = creer_chaines(gamma, nbChaines); // Crée un ensemble de chaînes avec les valeurs de gamma et nbChaines
    if(C==NULL){
        printf("Ensemble de chaines non créées, impossible de lire les données\n");
        return NULL;
    }

    // Lecture des chaînes et des points
    int numero, nbpoints;
    double x, y;
    char* donnee;
    for(int i=0; i<nbChaines; i++){
        // Lecture du numéro et du nombre de points avec strtok définissant l'espace comme délimiteur
        donnee = strtok(buffer, " ");
        sscanf(donnee, "%d", &numero); // Extrait le numéro de la chaîne
        donnee = strtok(NULL, " ");
        sscanf(donnee, "%d", &nbpoints); // Extrait le nombre de points dans la chaîne
        donnee = strtok(NULL, " "); // Passe au prochain token (saut de ligne)

        // Insertion de la chaîne dans l'ensemble de chaînes
        ins_en_tete_chaine(C, numero);

        // Lecture des points de la chaîne
        for(int j=0; j<nbpoints; j++){
            sscanf(donnee, "%lf", &x); // Extrait la coordonnée x du point
            donnee = strtok(NULL, " "); // Passe à la coordonnée y
            sscanf(donnee, "%lf", &y); // Extrait la coordonnée y du point
            donnee = strtok(NULL, " "); // Passe au prochain token (saut de ligne)

            ins_en_tete_point(C->chaines, x, y); // Insère le point dans la chaîne
        }

        fgets(buffer, 256, f); // Lecture de la prochaine ligne
    }

    return C;
}


//Ecriture d'un ensemble de chaines dans un fichier
void ecrireChaines(Chaines *C, FILE *f){
    if(f==NULL){
        printf("Fichier n'existe pas\n"); // Vérifie si le fichier est valide
        return;
    }

    if(C==NULL){
        printf("L'ensemble de chaines n'existe pas\n"); // Vérifie si l'ensemble de chaînes est valide
        return;
    }

    // Écriture des informations sur l'ensemble des chaînes dans le fichier
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f,"Gamma: %d\n", C->gamma);

    // Écriture des chaînes
    CellChaine* c = C->chaines;
    if(c==NULL){
        // Cas où il n'y a pas de chaînes dans l'ensemble de chaînes
        return;
    }

    char buffer[256], coordBuffer[256];
    CellPoint* p;
    int cpt;
    while(c){
        sprintf(buffer, ""); // Initialise la chaîne de caractères buffer
        cpt = 0;

        // Deuxième boucle parcourant tous les points pour les écrire dans le fichier
        p = c->points;
        while(p){
            sprintf(coordBuffer, "%.2f %.2f ", p->x, p->y); // Écriture des coordonnées du point dans coordBuffer
            strcat(buffer, coordBuffer); // Concaténation de coordBuffer à buffer
            cpt++;
            p = p->suiv;
        }

        // Écriture des données de la chaîne dans le fichier
        fprintf(f, "%d %d %s\n", c->numero, cpt, buffer);
        c = c->suiv;
    }
}



//Question 1.3
//Création du fichier SVG en html à partir d’un struct Chaines (donné sur moodle)
void afficheChainesSVG(Chaines *C, char* nomInstance){

    /*int i;*/ /*Variable non untilisée*/

    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

//Question 1.4
double longueurChaine(CellChaine *c){
    if(c==NULL){
        printf("Impossible de calculer la longueur de la chaîne qui n'existe pas\n"); // Vérifie si la chaîne est valide
        return -1;
    }

    CellPoint *a = c->points; // Initialise un pointeur de cellule de point au premier point de la chaîne
    if(a==NULL){
        printf("La chaîne n'a pas de points\n"); // Vérifie si la chaîne contient des points
        return 0;
    }

    CellPoint *b = a->suiv; // Initialise un pointeur de cellule de point au point suivant
    if(b==NULL){ // Vérifie si la chaîne contient plus d'un point
        printf("La chaîne n'a qu'un seul point\n"); // Vérifie si la chaîne contient plus d'un point
        return 0;
    }

    double xa, ya, xb, yb, longueur = 0; // Déclare des variables pour les coordonnées et la longueur
    while(b){
        xa = a->x; // Récupère la coordonnée x du point a
        ya = a->y; // Récupère la coordonnée y du point a
        xb = b->x; // Récupère la coordonnée x du point b
        yb = b->y; // Récupère la coordonnée y du point b

        longueur += sqrt(pow((xb-xa),2) + pow((yb-ya),2)); // Calcule la distance entre les points a et b et l'ajoute à la longueur totale

        a = a->suiv; // Passe au point suivant dans la chaîne
        b = b->suiv; // Passe au point suivant dans la chaîne
    }

    return longueur; 
}


double longueurTotale(Chaines *C){
    if(C==NULL){
        printf("Impossible de calculer la longueur d'un ensemble de chaînes qui n'existe pas\n"); // Vérifie si l'ensemble de chaînes est valide
        return -1;
    }

    double lc, longueur = 0; //variable pour la longueur de la chaîne actuelle et une variable pour la longueur totale
    CellChaine* c = C->chaines; // pointeur de cellule de chaîne au début de l'ensemble de chaînes
    while(c){
        lc = longueurChaine(c); // Calcule la longueur de la chaîne actuelle en appelant la fonction longueurChaine

        if(lc<0){
            printf("Erreur calcul longueur totale\n"); // Vérifie si le calcul de la longueur de la chaîne actuelle a échoué
            return -1;
        }else{
            longueur += lc; // Ajoute la longueur de la chaîne actuelle à la longueur totale
        }

        c = c->suiv; // Passe à la chaîne suivante dans l'ensemble de chaînes
    }

    return longueur;
}


int comptePointsTotal(Chaines *C){
    if(C==NULL){
        printf("Impossible de compter les points dans un ensemble de chaînes qui n'existe pas\n");
        return -1;
    }

    CellChaine* c = C->chaines; //pointeur de cellule de chaîne au début de l'ensemble de chaînes
    CellPoint* p;
    int cpt = 0; // Compteur de nombre de points 
    while(c){
        p = c->points; // Initialise le pointeur de cellule de point au début de la chaîne actuelle

        while(p){
            cpt++;
            p = p->suiv; // Passe au point suivant dans la chaîne
        }

        c = c->suiv; // Passe à la chaîne suivante dans l'ensemble de chaînes
    }

    return cpt;
}

/*Question 6.2*/
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax){
    Chaines* C = creer_chaines(3, nbChaines);
    if(C==NULL){
        printf("Chaines non créés pour generationAleatoire\n");
        return NULL;
    }

    srand(time(NULL));
    double x, y;

    //Boucle passant par toutes les chaines
    for(int i=1; i<=nbChaines; i++){
        ins_en_tete_chaine(C, i);

        //Boucle créant tous les points de la chaine
        for(int j=0; j<nbPointsChaine; j++){
            //Definition coordonnees du nouveau point
            x = xmax* ((double)rand() / (double)RAND_MAX);
            y = ymax* ((double)rand() / (double)RAND_MAX);
            
            ins_en_tete_point(C->chaines, x, y);
        }
    }

    return C;
}

