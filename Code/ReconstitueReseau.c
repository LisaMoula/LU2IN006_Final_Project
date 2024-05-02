//MOULA Lisa 21215357
//DUPART Guillaume 28710554


#include"ArbreQuat.h"

int main(int argc, char** argv){
    //Verification arguments
    if(argc!=3){
        printf("Usage: %s nom_fichier numero_structure\n", argv[0]);
        printf("1-Liste\n2-Table de hachage\n3-Arbre\n");
        return 1;
    }

    //Recuperation choix de la structure
    int choix;
    sscanf(argv[2], "%d", &choix);
    if(choix<1 || choix>3){
        printf("Numero de structure invalide. Voici les possibilités:\n");
        printf("1-Liste\n2-Table de hachage\n3-Arbre\n");
        return 1;
    }

    //Lecture fichier
    FILE* f= fopen(argv[1], "r");
    if(f==NULL){
        printf("Erreur ouverture fichier %s\n", argv[1]);
        return 2;
    }
    Chaines* C = lectureChaines(f);
    fclose(f);

    //Construction reseau avec structure choisie
    switch(choix){
        case 1: //Liste
            Reseau *R1 = reconstitueReseauListe(C);

            //Ecriture fichier
            FILE *f1 = fopen("testEcritureListe.res", "w");
            if(f1==NULL){
                printf("Erreur ouverture fichier testEcritureListe.res\n");
                return 2;
            }

            ecrireReseau(R1, f1);
            fclose(f1);

            //Affichage HTML
            afficheReseauSVG(R1, "affichageReseauListeSVG");

            liberer_Reseau(R1);
            break;

        case 2: //Table de Hachage
            printf("Veuillez indiquer la taille de la table de hachage souhaitée\n");

            char input[256];
            int m; //Taille de la table de hachage

            fgets(input, sizeof(input),stdin);
            if((sscanf(input, "%d", &m) != 1) || m<1){
                printf("Une table de hachage ne peut pas avoir une taille inférieure à 1\n");
                break;
            }else if(m>100){
                printf("Table de hachage inutilement grande\n");
                break;
            }


            Reseau *R2 = reconstitueReseauHachage(C, m);

            //Ecriture fichier
            FILE *f2 = fopen("testEcritureHachage.res", "w");
            if(f2==NULL){
                printf("Erreur ouverture fichier testEcritureHachage.res\n");
                return 2;
            }

            ecrireReseau(R2, f2);
            fclose(f2);

            //Affichage HTML
            afficheReseauSVG(R2, "affichageReseauHachageSVG");

            liberer_Reseau(R2);
            break;

        case 3: //ArbreQuat
            Reseau *R3 = reconstitueReseauArbre(C);

            //Ecriture fichier
            FILE *f3 = fopen("testEcritureArbre.res", "w");
            if(f3==NULL){
                printf("Erreur ouverture fichier testEcritureArbre.res\n");
                return 2;
            }

            ecrireReseau(R3, f3);
            fclose(f3);

            //Affichage HTML
            afficheReseauSVG(R3, "affichageReseauArbreSVG");

            liberer_Reseau(R3);
            break;
    }

    liberer_chaines(C);
}