

#include"ArbreQuat.h"

int main(int argc, char** argv){
    /*Warning*/
    printf("Attention, ce programme prend plusieurs heures a s'executer. Voulez vous continuer?\n[1: Oui / 0: Non]\n");
    
    char input[256];
    fgets(input, sizeof(input), stdin);
    int rep;

    if((sscanf(input, "%d", &rep) != 1) || rep == 0){
        printf("Arret du programme\n");
        return 0;
    }

    /*Calcul durée de chaque structure*/
    clock_t debut, fin;
    double tempsListe, tempsHach, tempsArbre;

    FILE *f = fopen("timedata.txt","w");
    if(f==NULL){
        printf("Erreur ouverture fichier timedata.txt\n");
        return 2;
    }


    for(int i=500; i<=5000; i+=500){
        Chaines *C = generationAleatoire(i, 100, 5000, 5000);

        /*Liste*/
        debut = clock();
        Reseau *R1 = reconstitueReseauListe(C);
        fin = clock();

        tempsListe = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        liberer_Reseau(R1);

        /*Table de hachage*/

        debut = clock();
        Reseau *R2 = reconstitueReseauHachage(C, i);
        fin = clock();

        tempsHach = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        liberer_Reseau(R2);

        /*Arbre quaternaire*/
        debut = clock();
        Reseau *R3 = reconstitueReseauArbre(C);
        fin = clock();

        tempsArbre = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        liberer_Reseau(R3);

        fprintf(f, "%d %f %f %f\n", comptePointsTotal(C), tempsListe, tempsHach, tempsArbre);

        printf("%d/10\n", i/500);

        liberer_chaines(C);
    }

    fclose(f);

    return 0;
}