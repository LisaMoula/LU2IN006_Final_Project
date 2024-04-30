#include<stdio.h>
#include"ArbreQuat.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <1 avec fichier\n -2 Avec chaines aléatoires\n>",argv[0]);
    }

    int choix;
    sscanf(argv[1], "%d", &choix);
    if(choix < 1 || choix > 2) {
        printf("Numéro invalide. Voici les possibilités:\n");
        printf("1-Fichier\n2-Chaines Aléatoires\n");
        return 1;
    }

    switch(choix) {
        case 1: { // avec un fichier
            printf("Veuillez entrer le nom du fichier à lire\n");
            char nom_fic[70];
            scanf("%s", nom_fic);
            FILE* fic_lecture = fopen(nom_fic, "r");
            if (!fic_lecture) {
                printf("Erreur d'ouverture du fichier donné\n");
                return 1;
            }
            printf("1\n");
            Chaines* C = lectureChaines(fic_lecture);
            int nb_points = comptePointsTotal(C);

            FILE* f = fopen("comparaison_temps.txt", "w");
            if (!f) {
                printf("Erreur d'ouverture du fichier décriture\n");
                return 1;
            }

            fprintf(f, "Nombre total de points : %d\n\n", nb_points);
            double tempsListe, tempsHach, tempsArbre;
            clock_t debut, fin;
            debut = clock();
            //avec Liste
            Reseau *R1 = reconstitueReseauListe(C);
            fin = clock();
            tempsListe = ((double)(fin - debut)) / CLOCKS_PER_SEC;
            liberer_Reseau(R1);
            fprintf(f, "Temps mis pour reconstituer le réseau avec la liste : %lf\n", tempsListe);
            //avec Arbre
            debut = clock();
            Reseau *R2 = reconstitueReseauArbre(C);
            fin = clock();
            tempsArbre = ((double)(fin - debut)) / CLOCKS_PER_SEC;
            liberer_Reseau(R2);
            fprintf(f, "Temps mis pour reconstituer le réseau avec l'arbre : %lf\n", tempsArbre);
            //hachage
            FILE* fich=fopen("hach.txt","w");
            for (int i=1; i<10*nb_points; i++) {
                debut = clock();
                Reseau *R3=reconstitueReseauHachage(C, i);
                fin = clock();
                tempsHach=((double)(fin - debut)) / CLOCKS_PER_SEC;
                liberer_Reseau(R3);
                fprintf(f, "Temps mis pour reconstituer le réseau avec une table de hachage de taille %d : %lf\n", i, tempsHach);
                fprintf(fich,"%d %f\n",i,tempsHach);
            }
            fclose(f);
            liberer_chaines(C);
            break;
        }
        case 2: {//avec des chaines aléatoires
            //Warning
            printf("Attention, ce programme prend plusieurs heures à s'executer. Voulez vous continuer?\n[1: Oui / 0: Non]\n");
            char input[256];
            fgets(input, sizeof(input), stdin);
            int rep;
            if((sscanf(input, "%d", &rep) != 1) || rep == 0) {
                printf("Arrêt du programme\n");
                return 0;
            }

            /*Calcul durée de chaque structure*/
            double tempsListe, tempsHach, tempsArbre;

            FILE *f = fopen("timedata.txt", "w");
            if(f == NULL) {
                printf("Erreur ouverture fichier timedata.txt\n");
                return 1;
            }

            for(int i=500; i<=5000; i+=500) {
                Chaines *C = generationAleatoire(i, 100, 5000, 5000);

                /*Liste*/
                clock_t debut ,fin;
                debut= clock();
                //Reseau *R1 = reconstitueReseauListe(C);
                //fin = clock();

                //tempsListe = ((double)(fin - debut)) / CLOCKS_PER_SEC;
                //liberer_Reseau(R1);

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

                fprintf(f, "%d %f %f\n", comptePointsTotal(C), tempsHach, tempsArbre);

                printf("%d/10\n", i/500);

                liberer_chaines(C);
            }
            fclose(f);
            break;
        }
    }
    return 0;
}
