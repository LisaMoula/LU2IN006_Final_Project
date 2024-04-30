

#include"Hachage.h"

TableHachage* creer_TableHachage(int m){
    // Vérifier que la taille de la table de hachage est valide
    if(m < 1){
        printf("Table de hachage ne peut pas avoir une taille nulle ou négative\n");
        return NULL;
    }

    // Allocation de mémoire pour la structure TableHachage
    TableHachage *H = (TableHachage*)malloc(sizeof(TableHachage));
    // Vérifier si l'allocation de mémoire a réussi
    if(H == NULL){
        printf("Erreur malloc TableHachage\n");
        return NULL;
    }

    // Initialisation des attributs de la table de hachage
    H->nbElement = 0; // Nombre d'éléments dans la table
    H->tailleMax = m; // Taille maximale de la table

    // Allocation de mémoire pour le tableau de pointeurs vers les listes chaînées
    H->T = (CellNoeud**)malloc(sizeof(CellNoeud*) * m);
    // Vérifier si l'allocation de mémoire a réussi
    if(H->T == NULL){
        printf("Erreur malloc H->T creation TableHachage\n");
        // En cas d'échec, libérer la mémoire allouée précédemment pour la structure TableHachage
        free(H);
        return NULL;
    }

    // Initialisation des pointeurs du tableau à NULL
    for(int i = 0; i < m; i++){
        H->T[i] = NULL;
    }

    // Renvoyer un pointeur vers la table de hachage nouvellement créée
    return H;
}


void liberer_TableHachage(TableHachage *H){
    // Vérifier si la table de hachage est déjà libérée
    if(H == NULL){
        printf("Table de Hachage déjà libérée\n");
        return;
    }

    CellNoeud *temp, *c;

    // Parcourir chaque élément du tableau de la table de hachage
    for(int i = 0; i < H->tailleMax; i++){
        c = H->T[i]; // Pointeur vers la tête de la liste chaînée à l'indice i
        temp = NULL;
        // Parcourir la liste chaînée
        while(c){
            temp = c; // Stocker le pointeur actuel avant de libérer la mémoire
            c = c->suiv; // Avancer au prochain élément de la liste
            liberer_CellNoeud(temp); // Libérer la mémoire de l'élément actuel
        }
    }

    // Libérer le tableau de pointeurs de la table de hachage
    free(H->T);
    // Libérer la mémoire de la structure de la table de hachage
    free(H);
}


//Obtention de la cle
int clef(double x, double y){
    return y + (x+y) * (x+y+1)/2;
}

//Obtention de la valeur de hachage
int hachage(int cle, int m){
    // Vérifie si la clé est négative
    if(cle < 0){
        printf("Impossible de hacher: la clé n'est pas valide\n");
        return -1; // Retourne une valeur d'erreur
    }

    // Définition de la constante A, dérivée de la formule de hachage
    double A = (sqrt(5) - 1) / 2;

    // Calcul des deux parties de la formule de hachage
    double var1 = cle * A;
    int var2 = cle * A;

    // Calcul de la valeur de hachage finale
    int res = m * (var1 - var2);

    // Retourne la valeur de hachage calculée
    return res;
}


Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    // Vérifie si le réseau et la table de hachage existent
    if(R == NULL){
        printf("Impossible de chercher le nœud dans un réseau qui n'existe pas\n");
        return NULL;
    }
    if(H == NULL){
        printf("Impossible de chercher le nœud dans une table de hachage qui n'existe pas\n");
        return NULL;
    }

    // Calcul de la clé à partir des coordonnées
    int cle = clef(x, y);

    // Calcul de la valeur de hachage pour cette clé
    int hach = hachage(cle, H->tailleMax);

    // Recherche du nœud dans la table de hachage
    CellNoeud *c = H->T[hach];

    // Boucle de recherche dans la liste des nœuds à l'index calculé dans la table de hachage
    while(c){
        if((c->nd->x == x) && (c->nd->y == y)){
            // Si le nœud est trouvé, retourne le nœud
            return c->nd;
        }
        c = c->suiv;
    }

    // Si le nœud n'est pas trouvé, on doit le créer et l'ajouter au réseau et à la table de hachage

    // Création du nœud
    Noeud *n = creer_Noeud((R->nbNoeuds)+1, x, y);
    if(n == NULL){ 
        printf("Erreur création du nœud dans rechercheCreeNoeudHachage\n");
        return NULL;
    }

    // Création de la cellule de nœud pour le réseau
    CellNoeud *c_reseau = creer_CellNoeud(n);
    if(c_reseau == NULL){ 
        printf("Erreur création de la cellule de nœud dans rechercheCreeNoeudHachage\n");
        liberer_Noeud(n);
        return NULL;
    }

    // Création de la cellule de nœud pour la table de hachage
    CellNoeud *c_hach = creer_CellNoeud(n);
    if(c_hach == NULL){ 
        printf("Erreur création de la cellule de nœud dans rechercheCreeNoeudHachage\n");
        liberer_CellNoeud(c_reseau);
        liberer_Noeud(n);
        return NULL;
    }

    // Ajout du nœud au réseau
    c_reseau->suiv = R->noeuds;
    R->noeuds = c_reseau;
    (R->nbNoeuds)++;

    // Ajout du nœud à la table de hachage
    c_hach->suiv = H->T[hach];
    H->T[hach] = c_hach;
    (H->nbElement)++;

    // Retourne le nœud créé
    return n;
}



//Reconstitution Reseau avec table de hachage a partir d'une Chaine
Reseau* reconstitueReseauHachage(Chaines *C, int M){
    // Vérifie si la chaîne existe
    if(C == NULL){
        printf("Impossible de construire le réseau à partir d'une chaîne qui n'existe pas\n");
        return NULL;
    }

    // Crée un nouveau réseau
    Reseau* R = creer_Reseau(C->gamma);
    if(R == NULL){ 
        printf("Erreur création du réseau dans reconstitueReseauHachage\n");
        return NULL;
    }

    // Crée une nouvelle table de hachage
    TableHachage *H = creer_TableHachage(M);
    if(H == NULL){
        printf("Erreur création de la table de hachage dans reconstitueReseauHachage\n");
        liberer_Reseau(R);
        return NULL;
    }

    CellChaine *c = C->chaines; // Chaînes
    CellPoint *prec, *p; // Points des chaînes
    Noeud *extrA, *extrB, *cour; // Noeuds: extrémités des commodités et le nœud courant
    Noeud *voisin; // Le nœud ajouté précédemment

    // Boucle parcourant toutes les chaînes
    for(int i = 0; i < C->nbChaines; i++){
        
        p = c->points;
        prec = NULL;
        voisin = NULL;
        // Boucle parcourant tous les points d'une chaîne
        while(p){
            // Recherche ou ajout du nœud correspondant au point de la chaîne
            cour = rechercheCreeNoeudHachage(R, H, p->x, p->y);
            if(cour == NULL){
                printf("Erreur recherche nœud lors de la reconstitution du réseau\n");
                liberer_Reseau(R);
                liberer_TableHachage(H);
                return NULL;
            }

            // Récupération de l'extrémité A de la commodité
            if(prec == NULL){
                extrA = cour;
            }

            // Mise à jour des voisins
            maj_voisins(R, voisin, cour);

            // Le voisin du prochain nœud est ce nœud
            voisin = cour;
            prec = p;
            p = p->suiv;
        }

        // Récupération de l'extrémité B de la commodité
        extrB = cour;

        // Création et ajout de la commodité
        CellCommodite* C = creer_CellCommodite(extrA, extrB);
        if(C == NULL){
            printf("Erreur création de la commodité dans reconstitueReseauListe\n");
            liberer_Reseau(R);
            liberer_TableHachage(H);
            return NULL;
        }
        C->suiv = R->commodites;
        R->commodites = C;

        c = c->suiv; // Passage à la chaîne suivante
    }

    // Libération de la table de hachage (utilisée uniquement ici)
    liberer_TableHachage(H);

    return R;
}
