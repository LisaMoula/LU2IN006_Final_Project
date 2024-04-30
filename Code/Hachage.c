

#include"Hachage.h"

TableHachage* creer_TableHachage(int m){
    if(m<1){
        printf("Table de hachage ne peut pas avoir taille nulle ou négative\n");
        return NULL;
    }

    TableHachage *H = (TableHachage*)malloc(sizeof(TableHachage));
    if(H==NULL){
        printf("Erreur malloc TableHachage\n");
        return NULL;
    }

    H->nbElement = 0;
    H->tailleMax = m;
    H->T = (CellNoeud**)malloc(sizeof(CellNoeud*)*m);
    if(H->T == NULL){
        printf("Erreur malloc H->T creation TableHachage\n");
        free(H);
        return NULL;
    }

    //Initialisation des pointeurs du tableau à NULL
    for(int i=0; i<m; i++){
        H->T[i] = NULL;
    }

    return H;
}

//Liberation Table de Hachage
void liberer_TableHachage(TableHachage *H){
    if(H==NULL){
        printf("Table de Hachage deja libérée\n");
        return;
    }

    CellNoeud *temp, *c;
    for(int i=0; i<(H->tailleMax); i++){
        c = H->T[i];
        temp = NULL;
        while(c){
            temp = c;
            c = c->suiv;
            liberer_CellNoeud(temp);
        }
    }

    free(H->T);
    free(H);
}

//Obtention de la cle
int clef(double x, double y){
    return y + (x+y) * (x+y+1)/2;
}

//Obtention de la valeur de hachage
int hachage(int cle, int m){
    if(cle<0){
        printf("Impossible de hacher: la clef n'est pas valide\n");
        return -1;
    }

    double A = (sqrt(5)-1)/2;

    double var1 = cle*A;
    int var2 = cle*A;

    int res = m*(var1 - var2);

    return res;
}

//Recherche / Création de noeud dans Reseau avec table de hachage
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    if(R==NULL){
        printf("Impossible de chercher noeud dans Reseau qui n'existe pas\n");
        return NULL;
    }
    if(H==NULL){
        printf("Impossible de chercher noeud dans table de hachage qui n'existe pas\n");
        return NULL;
    }

    //Recherche du noeud dans la table de Hachage
    int cle = clef(x, y);
    int hach = hachage(cle, H->tailleMax);

    CellNoeud *c = H->T[hach]; //Premier noeud au bon indice de la table de hachage

    //Boucle cherchant le noeud dans la liste des noeud au bon indice de la table
    while(c){
        if((c->nd->x == x) && (c->nd->y == y)){
            return c->nd;
        }

        c = c->suiv;
    }

    //Si nous sortons de la boucle, alors le noeud n'existe pas dans le réseau. On doit donc l'ajouter
    Noeud *n = creer_Noeud((R->nbNoeuds)+1, x, y);
    if(n==NULL){ 
        printf("Erreur création Noeud rechercheCreeNoeudHachage\n");
        return NULL;
    }
    //Création CellNoeud du reseau
    c = creer_CellNoeud(n);
    if(c==NULL){ 
        printf("Erreur création CellNoeud rechercheCreeNoeudListe\n");
        liberer_Noeud(n);
        return NULL;
    }
    //Création CellNoeud de la table de hachage
    CellNoeud *c_hach = creer_CellNoeud(n);
    if(c_hach==NULL){ 
        printf("Erreur création CellNoeud rechercheCreeNoeudListe\n");
        liberer_CellNoeud(c);
        liberer_Noeud(n);
        return NULL;
    }

    //Insertion noeud dans le reseau
    c->suiv = R->noeuds;
    R->noeuds = c;
    (R->nbNoeuds)++;

    //Insertion noeud dans la table de hachage
    c_hach->suiv = H->T[hach];
    H->T[hach] = c_hach;
    (H->nbElement)++;

    return n;
}


//Reconstitution Reseau avec table de hachage a partir d'une Chaine
Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if(C==NULL){
        printf("Impossible de construire reseau a partir d'une chaine qui n'existe pas\n");
        return NULL;
    }

    //Création Reseau
    Reseau* R = creer_Reseau(C->gamma);
    if(R==NULL){ 
        printf("Erreur création Réseau reconstitueReseauHachage\n");
        return NULL;
    }

    //Création Table de Hachage
    TableHachage *H = creer_TableHachage(M);
    if(H==NULL){
        printf("Erreur création Table de Hachage reconstitueReseauHachage\n");
        liberer_Reseau(R);
        return NULL;
    }

    CellChaine *c = C->chaines; //Chaines
    CellPoint *prec, *p; //Points des chaines
    Noeud *extrA, *extrB, *cour; //Noeuds: extremites des commodites et le noeud courant
    Noeud *voisin; //La cellule du noeud ajouté précédemment

    //Boucle passant par toutes les chaines (Boucle for est plus fiable)
    for(int i=0; i<(C->nbChaines); i++){
        
        p = c->points;
        prec = NULL;
        voisin = NULL;
        //Boucle passant par tous les points d'une chaine
        while(p){
            //Recherche ou ajout du noeud correspondant au point de la chaine
            cour = rechercheCreeNoeudHachage(R, H, p->x, p->y);
            if(cour == NULL){
                printf("Erreur recherche noeud dans reconstitution du reseau\n");
                liberer_Reseau(R);
                liberer_TableHachage(H);
                return NULL;
            }

            //Recuperation extremite A de la commodite
            if(prec == NULL){
                extrA = cour;
            }

            //Mise a jour des voisins
            maj_voisins(R, voisin, cour);

            //Le voisin du prochain noeud est ce noeud
            voisin = cour;
            prec = p;
            p = p->suiv;
        }

        //Recuperation extremite B de la commodite
        extrB = cour;

        //Creation et ajout de la commodite
        CellCommodite* C = creer_CellCommodite(extrA, extrB);
        if(C==NULL){
            printf("Erreur création commodite reconstitueReseauListe\n");
            liberer_Reseau(R);
            liberer_TableHachage(H);
            return NULL;
        }
        C->suiv = R->commodites;
        R->commodites = C;

        c = c->suiv; //Passage a la chaine suivante
    }

    //Liberation table de hachage (utilisée que ici)
    liberer_TableHachage(H);

    return R;
}