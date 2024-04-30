

CC = gcc
CFLAGS = -Wno-format-zero-length -g


all: ChaineMain ReconstitueReseau HachageMain main


#Général
SVGwriter.o: SVGwriter.c
	$(CC) $(CFLAGS) -c $^

#Reseau
ReconstitueReseau: ReconstitueReseau.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

ReconstitueReseau.o: ReconstitueReseau.c
	$(CC) $(CFLAGS) -c $^

Reseau.o: Reseau.c
	$(CC) $(CFLAGS) -c $^

#Graphe
Graphe.o: Graphe.c
	$(CC) $(CFLAGS) -c $^

GrapheMain.o: GrapheMain.c
	$(CC) $(CFLAGS) -c $^

GrapheMain: GrapheMain.o Chaine.o Reseau.o ArbreQuat.o Graphe.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

#ArbreQuat
ArbreQuat.o: ArbreQuat.c
	$(CC) $(CFLAGS) -c $^

#Hachage
HachageMain: HachageMain.o Chaine.o Reseau.o Hachage.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

HachageMain.o: HachageMain.c
	$(CC) $(CFLAGS) -c $^

Hachage.o: Hachage.c
	$(CC) $(CFLAGS) -c $^

#Chaine

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

ChaineMain.o: ChaineMain.c
	$(CC) $(CFLAGS) -c $^

Chaine.o: Chaine.c
	$(CC) $(CFLAGS) -c $^

#Calcul du temps
main.o: main.c
	$(CC) $(CFLAGS) -c $^

main: Chaine.o Reseau.o Hachage.o ArbreQuat.o main.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

graphtemps: main
	./main
	gnuplot -p < commande.txt

#Nettoyage
clean:
	rm -f *.o *.html *Main ReconstitueReseau test* timedata.txt main courbesVitesse*

