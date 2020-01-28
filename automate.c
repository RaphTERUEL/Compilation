#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE}bool;

typedef struct vertex vertex;
typedef struct edge edge;
typedef struct automate automate;

struct edge{
	vertex *nextVertex;
	char word;
};

struct vertex{
	int numSommet;
	edge **tab_edge;
	int nb_edge;
	bool accepteur;
};

struct automate{
	vertex *nextVertex;
	int nbr_etats;
};


void addVertex(vertex *vert, automate *autoFiniNonDeter, char carac);
void affichage(automate autoFiniNonDeter);

int main(int nbarg, char *args[])
{
	automate autoFiniNonDeter;
	autoFiniNonDeter.nbr_etats=1;
	vertex v;
	v.numSommet=0;
	v.nb_edge=0;
	autoFiniNonDeter.nextVertex=&v;
	addVertex(&v, &autoFiniNonDeter, 'a');
	affichage(autoFiniNonDeter);


	return 0;
}

void addVertex(vertex *vert, automate *autoFiniNonDeter, char carac) {
	vertex v;
	edge E;
	v.nb_edge=0;
	v.tab_edge=NULL;
	v.numSommet=autoFiniNonDeter->nbr_etats;
	autoFiniNonDeter->nbr_etats++;
	vert->nb_edge++;
	vert->tab_edge=(edge**)malloc(sizeof(edge*)*vert->nb_edge);
	vert->tab_edge[vert->nb_edge-1]=&E;
	E.nextVertex=&v;
	E.word=carac;

}

void affichage(automate autoFiniNonDeter){
	vertex v;
	int i;
	v=*autoFiniNonDeter.nextVertex;
	while(v.tab_edge!=NULL){
		for (i = 0; i< v.nb_edge; i++){
			printf("%d --> %c --> %d\n",v.numSommet,v.tab_edge[i]->word,v.tab_edge[i]->nextVertex->numSommet);
		}
		v=*v.tab_edge[i-1]->nextVertex;
	}

}
