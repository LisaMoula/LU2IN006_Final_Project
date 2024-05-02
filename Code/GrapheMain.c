//MOULA Lisa 21215357
//DUPART Guillaume 28710554

#include"Graphe.h"

int main(int argc, char** argv){
    if(argc<2){
        printf("Trop peu d'arguments\nUsage: %s nom_fichier\n", argv[0]);
        return 1;
    }

    /*Creation chaines puis reseau puis graphe*/
    FILE *f = fopen(argv[1], "r");
    if(f==NULL){
        printf("Erreur ouverture fichier %s\n", argv[1]);
        return 2;
    }
    Chaines *C = lectureChaines(f);

    fclose(f);
    
    Reseau *r = reconstitueReseauArbre(C);
    Graphe *g = creerGraphe(r);

    /*Test fonctions graphe*/
    int sd = 10, sf = 11;
    S_file* chemin = ppChemin(g, sd, sf);
    printf("Chemin entre sommet %d et sommet %d est: ", sd, sf);
    int s;
    while((chemin != NULL) && (!(estFileVide(chemin)))){
        s = defile(chemin);
        printf("%d ",s);
    }
    free(chemin);
    printf("\n\n");

    int test = reorganiseReseau(r);
    printf("Resultat de reorganiseReseau: %d\n", test);

    liberer_chaines(C);
    liberer_Reseau(r);
    libererGraphe(g);

    return 0;
}