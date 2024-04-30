

#include"Reseau.h"

//Fonctions de création et libération
//Création Noeud
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
void liberer_Noeud(Noeud* n){
    if(n==NULL){
        printf("Noeud deja libéré\n");
        return;
    }

    CellNoeud *temp, *c = n->voisins;
    while(c){
        temp = c;
        c = c->suiv;
        liberer_CellNoeud(temp);
    }
    free(n);
}


//Création CellNoeud
CellNoeud* creer_CellNoeud(Noeud* n){
    CellNoeud *c = (CellNoeud*)malloc(sizeof(CellNoeud));
    if(c==NULL){
        printf("Erreur malloc CellNoeud\n");
        return NULL;
    }

    c->nd = n;
    c->suiv = NULL;

    return c;
}

//Liberation CellNoeud
void liberer_CellNoeud(CellNoeud *c){
    if(c==NULL){
        printf("CellNoeud déjà libéré\n");
        return;
    }

    c->nd = NULL;
    free(c);
}


//Création CellCommodité
CellCommodite* creer_CellCommodite(Noeud* extrA, Noeud* extrB){
    if(extrA == NULL || extrB == NULL){
        printf("Erreur création CellCommodite: extremité n'existe pas\n");
        return NULL;
    }

    CellCommodite* C = (CellCommodite*)malloc(sizeof(CellCommodite));
    if(C==NULL){
        printf("Erreur malloc CellCommodite\n");
        return NULL;
    }

    C->extrA = extrA;
    C->extrB = extrB;

    return C;
}

//Libération CellCommodite
void liberer_CellCommodite(CellCommodite *C){
    if(C==NULL){
        printf("Commodite déjà libérée\n");
        return;
    }

    C->extrA = NULL;
    C->extrB = NULL;

    free(C);
}


//Création Réseau
Reseau* creer_Reseau(int gamma){
    Reseau* r = (Reseau*)malloc(sizeof(Reseau));
    if(r==NULL){
        printf("Erreur malloc Reseau\n");
        return NULL;
    }

    r->nbNoeuds = 0;
    r->gamma = gamma;
    r->noeuds = NULL;
    r->commodites = NULL;

    return r;
}

//Libération Reseau
void liberer_Reseau(Reseau *R){
    if(R==NULL){
        printf("Reseau deja libéré\n");
        return;
    }

    //Liberation Noeuds
    CellNoeud *tempc, *c = R->noeuds;
    while(c){
        tempc = c;
        c = c->suiv;
        liberer_Noeud(tempc->nd);
        liberer_CellNoeud(tempc);
    }

    //Liberation Commodites
    CellCommodite *tempC, *C = R->commodites;
    while(C){
        tempC = C;
        C = C->suiv;
        liberer_CellCommodite(tempC);
    }

    free(R);
}



//Question 2.1
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    if(R==NULL){
        printf("Impossible de chercher noeud: Reseau non créé\n");
        return NULL;
    }

    //Recherche du noeud dans le réseau

    CellNoeud *c = R->noeuds, *prec = NULL;
    for(int i=0; (i < R->nbNoeuds) && (c->nd->x <= x); i++){
        if(c->nd->x == x){
            if(c->nd->y == y){
                return c->nd;
            }else if(c->nd->y > y){
                break;
            }
        }

        prec = c;

        c = c->suiv;
    }

    //Cas où nous sortons de la boucle: le noeud n'existe pas dans le réseau. On doit donc le créer puis l'ajouter
    Noeud *n = creer_Noeud((R->nbNoeuds)+1, x, y);
    if(n==NULL){ 
        printf("Erreur création Noeud rechercheCreeNoeudListe\n");
        return NULL;
    }

    CellNoeud* cn = creer_CellNoeud(n);
    if(cn==NULL){ 

        printf("Erreur création CellNoeud rechercheCreeNoeudListe\n");
        liberer_Noeud(n);
        return NULL;
    }


    /*Ajout du noeud de manière a garder la liste de noeuds créés triée*/
    if(prec==NULL){
        cn->suiv = R->noeuds;
        R->noeuds = cn;
    }else{
        cn->suiv = c;
        prec->suiv = cn;
    }

    (R->nbNoeuds)++;

    return n;
}

//Question 2.2
void maj_voisins(Reseau *R, Noeud *v, Noeud *n){
    if(n==NULL){
        printf("Impossible de mettre a jour noeud qui n'existe pas\n");
        return;
    }

    if(v==NULL){ //Cas ou il n'y a pas de voisin à ajouter
        return;
    }

    //On vérifie si l'on a pas déjà ajouté ce voisin (par moyen d'une autre chaine)
    CellNoeud *dejala = n->voisins;
    while(dejala){
        if(dejala->nd->num == v->num){
            //Si le voisin a deja été ajouté, alors on sort de la fonction
            return;
        }
        dejala = dejala->suiv;
    }

    //Ajout du noeud voisin dans la liste des voisins
    CellNoeud *voisin = creer_CellNoeud(v);
    voisin->suiv = n->voisins;
    n->voisins = voisin;

    //Mise a jour des voisins du voisin
    CellNoeud *vois_du_vois = creer_CellNoeud(n);
    vois_du_vois->suiv = v->voisins;
    v->voisins = vois_du_vois;
}

Reseau* reconstitueReseauListe(Chaines *C){
    if(C==NULL){
        printf("Impossible de construire reseau a partir d'une chaine qui n'existe pas\n");
        return NULL;
    }

    Reseau* R = creer_Reseau(C->gamma);
    if(R==NULL){ 
        printf("Erreur création Réseau reconstitueReseauListe\n");
        return NULL;
    }

    CellChaine *c = C->chaines; //Chaines
    CellPoint *prec, *p; //Points des chaines
    Noeud *extrA, *extrB, *cour; //Noeuds: extremites des commodites et le noeud courant
    Noeud *voisin = NULL; //La cellule du noeud ajouté précédemment

    //Boucle passant par toutes les chaines (Boucle for est plus fiable)
    for(int i=0; i<(C->nbChaines); i++){
        //Boucle passant par tous les points d'une chaine
        p = c->points;
        prec = NULL;
        voisin = NULL;
        while(p){
            //Recherche ou ajout du noeud correspondant au point de la chaine
            cour = rechercheCreeNoeudListe(R, p->x, p->y);
            if(cour == NULL){
                printf("Erreur recherche noeud dans reconstitution du reseau\n");
                liberer_Reseau(R);
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
            return NULL;
        }
        C->suiv = R->commodites;
        R->commodites = C;

        c = c->suiv;
    }

    return R;
}

//Comptage des liaisons
int nbLiaisons(Reseau *R){
    if(R==NULL){
        printf("Impossible de compter nombre de liaisons d'un réseau qui n'existe pas\n");
        return -1;
    }

    int cpt = 0;
    Noeud* dejaVu[R->nbNoeuds]; //Tableau pour stocker les noeuds deja vus
    int vu = 0; //int servant comme booléen pour vérifier si noeud a été vu ou non

    CellNoeud *vois, *c = R->noeuds;
    //Boucle passant par tous les noeuds du Reseau
    for(int i=0; i<(R->nbNoeuds); i++){
        vois = c->nd->voisins;

        //Boucle passant par tous les voisins du noeud
        while(vois){
            vu = 0;
            //Boucle passant par les noeuds deja vus
            for(int j=0; j<i; j++){
                if(dejaVu[j] == vois->nd){
                    vu = 1;
                    break;
                }
            }

            if(!(vu)){
                //Si ce noeud n'a pas été vu, on incremente le compteur de liaisons
                cpt++;
            }

            vois = vois->suiv;
        }

        dejaVu[i] = c->nd;
        c = c->suiv;
    }

    return cpt;
}

//Comptage commodites
int nbCommodites(Reseau *R){
    if(R==NULL){
        printf("Impossible de compter nombre de liaisons d'un réseau qui n'existe pas\n");
        return -1;
    }

    int cpt = 0;
    CellCommodite *C = R->commodites;
    while(C){
        cpt++;
        C = C->suiv;
    }

    return cpt;
}


//Ecriture du reseau
void ecrireReseau(Reseau *R, FILE *f){
    if(R==NULL){
        printf("Impossible d'ecrire reseau qui n'existe pas\n");
        return;
    }
    if(f==NULL){
        printf("Fichier d'ecriture ne s'est pas ouvert\n");
        return;
    }

    //Ecriture informations générales du réseau
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "nbCommodites: %d\n", nbCommodites(R));
    fprintf(f,"Gamma: %d\n\n", R->gamma);

    //Ecriture des noeuds et des liaisons du reseau
    //On reprend des instructions similaires à celles du comptage de liaisons
    Noeud* dejaVu[R->nbNoeuds]; //Tableau pour stocker les noeuds deja vus
    int vu = 0; //int servant comme booléen pour vérifier si noeud a été vu ou non

    CellNoeud *c = R->noeuds, *vois;
    char liaisbuffer[256]; //Buffer pour stocker ecriture des liaisons
    char buffer[256] = ""; //Buffer initial sur lequel les liaisons seront concaténées

    //Boucle passant par tous les noeuds du Reseau
    for(int i=0; i<(R->nbNoeuds); i++){
        //On ecrit le noeud
        fprintf(f, "v %d %lf %lf\n", c->nd->num, c->nd->x, c->nd->y);

        vois = c->nd->voisins;
        //Boucle passant par tous les voisins du noeud
        while(vois){
            vu = 0;
            //Boucle passant par les noeuds deja vus
            for(int j=0; j<i; j++){
                if(dejaVu[j] == vois->nd){
                    vu = 1;
                    break;
                }
            }

            if(!(vu)){
                //Si ce noeud n'a pas été vu, on ecrit la liaison
                sprintf(liaisbuffer, "l %d %d\n", vois->nd->num, c->nd->num);
                strcat(buffer, liaisbuffer);
            }

            vois = vois->suiv;
        }

        dejaVu[i] = c->nd;
        c = c->suiv;
    }
    fprintf(f, "\n%s\n", buffer);

    //Ecriture des commodites
    CellCommodite *C = R->commodites;
    while(C){
        fprintf(f, "k %d %d\n", C->extrA->num, C->extrB->num);
        C = C->suiv;
    }
}

//Ecriture du fichier SVG a partir du reseau
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}