#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {FALSE, TRUE}bool;
#define couleur(param) printf("\033[%sm",param)

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
	vertex *firstVertex;
	int nbr_etats;
};

void addVertex(vertex *vert, automate *autoFiniNonDeter, char carac, bool accept);
void affichage(automate autoFiniNonDeter);
automate* motVide();
automate * langagevide(void);
automate* mot(char *mot);
automate* concatenation(char *mot, char*mot2);
automate* union_(char *mot1, char *mot2);

int main(int nbarg, char *args[])
{
	automate * autoFiniNonDeter=mot("salut");

	affichage(*autoFiniNonDeter);

  autoFiniNonDeter = concatenation("abc","def");

  autoFiniNonDeter = union_("abc","def");


	return 0;
}

void addVertex(vertex *vert, automate *autoFiniNonDeter, char carac, bool accept) {
	vertex *v=malloc(sizeof(vertex));
	edge*  E=malloc(sizeof(edge));
	v->nb_edge=0;
	v->tab_edge=NULL;
	v->numSommet=autoFiniNonDeter->nbr_etats;
	autoFiniNonDeter->nbr_etats++;
	v->accepteur = accept;
	vert->nb_edge++;
	vert->tab_edge=(edge**)malloc(sizeof(edge*)*vert->nb_edge);
	vert->tab_edge[vert->nb_edge-1]=E;
	E->nextVertex=v;
	E->word=carac;
	return;
}

void affichage(automate autoFiniNonDeter){
	vertex *v=malloc(sizeof(vertex));
	int i;
	int cpt = 1;
  int cpt2=0;
	v=autoFiniNonDeter.firstVertex;
	while (v->tab_edge!=NULL){
		for (i = 0; i< v->nb_edge; i++){
			cpt++;
			if (v->accepteur)
				couleur("31");
			printf("%d ", v->numSommet);
			couleur("0");
			printf("--> %c -->",v->tab_edge[i]->word);
			if (v->tab_edge[i]->nextVertex->accepteur)
				couleur("31");
			printf(" %d\n",v->tab_edge[i]->nextVertex->numSommet);
			couleur("0");
		}
    if (v->tab_edge!=NULL || cpt<=autoFiniNonDeter.nbr_etats)
		  v=v->tab_edge[0]->nextVertex;
    else
    {
      cpt2++;
      v=v->tab_edge[cpt2]->nextVertex;
    }

	}
	couleur("0");
  printf("\n");
	return;
}

automate* motVide()
{
	automate * autoFiniNonDeter=malloc(sizeof(automate));
	autoFiniNonDeter->nbr_etats=1;
	autoFiniNonDeter->firstVertex=malloc(sizeof(vertex));
  autoFiniNonDeter->firstVertex->accepteur = TRUE;
	autoFiniNonDeter->firstVertex->nb_edge = 0;
	autoFiniNonDeter->firstVertex->tab_edge = NULL;
	return autoFiniNonDeter;
}

automate * langagevide(void){
	automate * new=malloc(sizeof(automate));
	new->nbr_etats=1;
	new->firstVertex=malloc(sizeof(vertex));
	new->firstVertex->accepteur = FALSE;
	new->firstVertex->nb_edge = 0;
	new->firstVertex->tab_edge = NULL;
	return new;
}


//renvoie un automate standard reconnaissant le langage composé
//d’un mot d’un caractère passé en paramètre
automate* mot(char *mot)
{
	automate * new=langagevide();
	vertex *v=new->firstVertex;
	for (int i = 0; i < strlen(mot); i++) {
		if(i==strlen(mot)-1)
			addVertex(v,new,mot[i],TRUE);
		else
			addVertex(v,new,mot[i],FALSE);

		v=v->tab_edge[0]->nextVertex;
	}


  return new;
}

automate* concatenation(char *mot1, char*mot2)
{

	automate * automate1 = mot(mot1);
  automate * automate2 = mot(mot2);

  automate * autoConcat = langagevide();

  vertex *v=automate1->firstVertex;

  autoConcat->nbr_etats = automate1->nbr_etats + automate2->nbr_etats - 1;
	autoConcat->firstVertex = automate1->firstVertex;

  //ajouter les sommets de automate1 à autoConcat
	while (v->tab_edge!=NULL) {
    v=v->tab_edge[0]->nextVertex;
	}
	v->tab_edge=(edge**)malloc(sizeof(edge*));
	v->nb_edge=1;
	v->tab_edge[0]=automate2->firstVertex->tab_edge[0];

  for (int i = automate1->nbr_etats; i < autoConcat->nbr_etats; i++) {

		v->tab_edge[0]->nextVertex->numSommet = i;
		v=v->tab_edge[0]->nextVertex;

	}

	free(automate1);
	free(automate2);


  affichage(*autoConcat);

  return autoConcat;

}

automate* union_(char *mot1, char *mot2)
{
  automate * automate1 = mot(mot1);
  automate * automate2 = mot(mot2);

  affichage(*automate1);
  affichage(*automate2);

  automate * autoUnion = langagevide();

  printf("ici1\n");
  autoUnion->nbr_etats = automate1->nbr_etats + automate2->nbr_etats - 1;
  printf("automate total : %d\n", autoUnion->nbr_etats);
  vertex *v = autoUnion->firstVertex;
  printf("ici3\n");
  v->tab_edge=(edge**)malloc(sizeof(edge*)*2);
  v->tab_edge[0]=automate1->firstVertex->tab_edge[0];
  v->tab_edge[1]=automate2->firstVertex->tab_edge[0];
	v->nb_edge=2;

	vertex *v1 = automate1->firstVertex;
	vertex *v2 = automate2->firstVertex;

  printf("ici4\n");
  for (int i = 1; i < automate1->nbr_etats; i++) {
    printf("ici4.5\n");
		v1->tab_edge[0]->nextVertex->numSommet = i;
    printf("v1 word : %c\n",v1->tab_edge[0]->word);
		v1=v1->tab_edge[0]->nextVertex;

	}
  printf("ici5\n");
  for (int i = automate1->nbr_etats; i < autoUnion->nbr_etats; i++) {
    printf("ici5.5\n");
		v2->tab_edge[0]->nextVertex->numSommet = i;
		v2=v2->tab_edge[0]->nextVertex;

	}
  printf("ici6\n");
  free(automate1);
	free(automate2);

  affichage(*autoUnion);

  return autoUnion;
}
