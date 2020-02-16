#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Permet de faire le type booléen
typedef enum {FALSE, TRUE}bool;
// Permet d'afficher des couleurs sur le terminal
#define couleur(param) printf("\033[%sm",param)

typedef struct vertex vertex;
typedef struct edge edge;
typedef struct automate automate;
typedef struct Enslist Enslist;
typedef struct Trans Trans;
typedef struct List List;

struct edge{
	vertex *nextVertex;
	char word;
};

struct vertex{
	int numSommet;
	edge *tab_edge;
	int nb_edge;
	bool accepteur;
};

struct automate{
	vertex *Vertex;
	int nbr_etats;
};




struct Trans
{
    int *sommet;
    int size;
    char carra;
    Trans *next;
};

struct Enslist
{
    int *sommet;
    int size;
    Enslist *next;
    Trans * First;
    int nb_T;
};

struct List
{
    Enslist *first;
    int nb;
};




automate * reuninon(automate * a, automate * b);
automate * concatenation(automate * a, automate * b);
void addEdge(vertex *a,vertex * b, char carac) ;
void affichage(automate autoFiniNonDeter);
automate* motVide();
automate * langagevide(void);
automate* caracter(char c);
automate * kleen(automate * a);
Enslist * ajouter_Trans(Enslist * e,char car,int voisin);
int vExiste(List * l, int* e, int nb);
Enslist * Derniermaillon(List * list);
automate * Determinisation(automate * a);
int charintab(char*car,char c,int nb);
char *transitiondifferente(automate* a, char *car);
int transitionexiste(vertex *v, char c);
bool tableauegale(int * a, int * b,int nb);
automate * minimisation(automate *a);
int nbtransition(automate *a, char *car);
void executionautomate(automate * a, char * mots);


#endif