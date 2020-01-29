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
	edge *tab_edge;
	int nb_edge;
	bool accepteur;
};

struct automate{
	vertex *Vertex;
	int nbr_etats;
};

automate * reuninon(automate * a, automate * b);
automate * concatenation(automate * a, automate * b);
void addEdge(vertex *a,vertex * b, automate *autoFiniNonDeter, char carac) ;
void affichage(automate autoFiniNonDeter);
automate* motVide();
automate * langagevide(void);
automate* caracter(char *c);


int main(int nbarg, char *args[])
{

	automate * a=caracter("a");
	automate * b=caracter("b");
    automate * d=reuninon(a,b);
    affichage(*a);
	automate * c=concatenation(d,a);

    affichage(*c);

	return 0;
}

void addEdge(vertex *a,vertex * b, automate *autoFiniNonDeter, char carac) {

	edge  E;
    E.nextVertex=b;
    E.word=carac;

	a->nb_edge++;

	E.nextVertex=b;
	E.word=carac;
    a->tab_edge=(edge*)realloc(a->tab_edge,sizeof(edge)*a->nb_edge);
    a->tab_edge[a->nb_edge-1]=E;

}


void affichage(automate a){

    int i;
    int cpt = 1;


    for (int j = 0; j < a.nbr_etats ; j++) {

        for (i = 0; i< a.Vertex[j].nb_edge; i++){
            cpt++;
            if (a.Vertex[j].accepteur)
                couleur("31");
            printf("%d ", a.Vertex[j].numSommet);
            couleur("0");
            printf("--> %c -->",a.Vertex[j].tab_edge[i].word);
            if (a.Vertex[j].tab_edge[i].nextVertex->accepteur)
                couleur("31");
            printf(" %d\n",a.Vertex[j].tab_edge[i].nextVertex->numSommet);
            couleur("0");
        }


    }
    couleur("0");
    printf("\n\n");
    return;
}


automate* motVide()
{
    automate * autoFiniNonDeter=malloc(sizeof(automate));
    autoFiniNonDeter->nbr_etats=1;
    autoFiniNonDeter->Vertex=malloc(sizeof(vertex));
    autoFiniNonDeter->Vertex[0].accepteur = TRUE;
    autoFiniNonDeter->Vertex[0].nb_edge = 0;
    autoFiniNonDeter->Vertex[0].tab_edge = NULL;
    return autoFiniNonDeter;
}

automate * langagevide(void){
    automate * new=malloc(sizeof(automate));
    new->nbr_etats=1;
    new->Vertex=malloc(sizeof(vertex));
    new->Vertex[0].accepteur = FALSE;
    new->Vertex[0].nb_edge = 0;
    new->Vertex[0].tab_edge = NULL;
    return new;
}


//renvoie un automate standard reconnaissant le langage composé
//d’un mot d’un caractère passé en paramètre
automate* caracter(char * c)
{

    automate * new=malloc(sizeof(automate));
    new->nbr_etats=2;

    new->Vertex=(vertex*)malloc(2*sizeof(vertex));

    new->Vertex[0].accepteur = FALSE;
    new->Vertex[0].nb_edge = 0;
    new->Vertex[0].numSommet = 0;
    new->Vertex[0].tab_edge = NULL;


    new->Vertex[1].accepteur = TRUE;
    new->Vertex[1].nb_edge = 0;
    new->Vertex[1].numSommet = 1;
    new->Vertex[1].tab_edge = NULL;

    addEdge(&new->Vertex[0],&new->Vertex[1],new,c[0]);



    return new;
}

automate * reuninon(automate * a, automate * b){
    int decalage=0;

    automate * new=malloc(sizeof(automate));
    new->nbr_etats= a->nbr_etats+b->nbr_etats -1;
    new->Vertex=(vertex*)malloc(new->nbr_etats * sizeof(vertex));

    for (int i = 0; i <a->nbr_etats ; ++i) {
        new->Vertex[i].numSommet = i;
        new->Vertex[i].accepteur = a->Vertex[i].accepteur;
    }
    for (int i = 0; i <a->nbr_etats ; ++i) {

        for (int j = 0; j <a->Vertex[i].nb_edge ; ++j) {
            addEdge(&new->Vertex[i],&new->Vertex[a->Vertex[i].tab_edge[j].nextVertex->numSommet],new,a->Vertex[i].tab_edge[j].word);
        }
    }
    decalage=a->nbr_etats;
    new->Vertex[0].accepteur=a->Vertex[0].accepteur | b->Vertex[0].accepteur;

    for (int j = 0; j <b->Vertex[0].nb_edge ; ++j) {
        addEdge(&new->Vertex[0],&new->Vertex[b->Vertex[0].tab_edge[j].nextVertex->numSommet+decalage-1],new,b->Vertex[0].tab_edge[j].word);
    }

    for (int i = decalage; i <b->nbr_etats+decalage ; i++) {

        new->Vertex[i].numSommet = i;
        new->Vertex[i].accepteur = b->Vertex[i-decalage+1].accepteur;

    }
    for (int i = decalage; i <b->nbr_etats-decalage ; i++) {
        for (int j = 0; j <b->Vertex[i-decalage+1].nb_edge ; j++) {
            addEdge(&new->Vertex[i],&new->Vertex[b->Vertex[i-decalage+1].tab_edge[j].nextVertex->numSommet],new,b->Vertex[i-decalage+1].tab_edge[j].word);

        }

    }
    printf("lqkhf\n");
    return new;

}

int nbaccepteur(automate * a){
    int count=0;
    for (int i = 0; i <a->nbr_etats ; ++i) {
        if(a->Vertex[i].accepteur==TRUE)
            count++;
    }

    return count;
}

automate * concatenation(automate * a, automate * b){
    int decalage=0;

    automate * new=malloc(sizeof(automate));
    new->nbr_etats= a->nbr_etats+ (nbaccepteur(a) * (b->nbr_etats -1));
    new->Vertex=(vertex*)malloc(new->nbr_etats * sizeof(vertex));

    for (int i = 0; i <a->nbr_etats ; ++i) {
        new->Vertex[i].numSommet = i;
        new->Vertex[i].accepteur = a->Vertex[i].accepteur;
    }
    for (int i = 0; i <a->nbr_etats ; ++i) {

        for (int j = 0; j <a->Vertex[i].nb_edge ; ++j) {
            addEdge(&new->Vertex[i],&new->Vertex[a->Vertex[i].tab_edge[j].nextVertex->numSommet],new,a->Vertex[i].tab_edge[j].word);
        }
    }
    decalage=a->nbr_etats;

    for (int j = 0; j <a->nbr_etats ; j++) {
        if(new->Vertex[j].accepteur==TRUE){
            new->Vertex[j].accepteur=b->Vertex[0].accepteur;
            for (int k = 0; k <b->Vertex[0].nb_edge ; k++) {
                addEdge(&new->Vertex[j],&new->Vertex[b->Vertex[0].tab_edge[k].nextVertex->numSommet+decalage-1],new,b->Vertex[0].tab_edge[k].word);
            }
            for (int i = decalage; i <b->nbr_etats+decalage ; i++) {
                new->Vertex[i].numSommet = i;
                new->Vertex[i].accepteur = b->Vertex[i-decalage+1].accepteur;
            }
            for (int i = decalage; i <b->nbr_etats-decalage ; i++) {
                for (int k = 0; k <b->Vertex[i-decalage+1].nb_edge ; k++) {
                    addEdge(&new->Vertex[i],&new->Vertex[b->Vertex[i-decalage+1].tab_edge[k].nextVertex->numSommet],new,b->Vertex[i-decalage+1].tab_edge[k].word);
                }
            }

            decalage+=b->nbr_etats-1;
        }
    }


    return new;

}