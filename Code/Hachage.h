

#include"Reseau.h"

typedef struct{
  int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

TableHachage* creer_TableHachage(int m);
void liberer_TableHachage(TableHachage *H);

//Question 4.2
int clef(double x, double y);
//Question 4.3
int hachage(int cle, int m);

//Question 4.4
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);

//Question 4.5
Reseau* reconstitueReseauHachage(Chaines *C, int M);