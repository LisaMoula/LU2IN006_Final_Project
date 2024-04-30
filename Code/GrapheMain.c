
#include"Graphe.h"

int main(){
    FILE *f = fopen("00014_burma.cha", "r");
    if(f==NULL){
        printf("Erreur ouverture fichier 00014_burma.cha\n");
        return 1;
    }
    Chaines *C = lectureChaines(f);

    fclose(f);
    
    Reseau *r = reconstitueReseauArbre(C);
    Graphe *g = creerGraphe(r);

    liberer_chaines(C);
    liberer_Reseau(r);
    libererGraphe(g);

    return 0;
}