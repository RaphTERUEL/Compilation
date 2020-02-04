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




typedef struct Trans
{
    int *sommet;
    int size;
    char carra;
    struct Trans *next;
}Trans;

typedef struct Enslist
{
    int *sommet;
    int size;
    struct Enslist *next;
    Trans * First;
    int nb_T;
}Enslist;

typedef struct
{
    Enslist *first;
    int nb;
}List;




automate * reuninon(automate * a, automate * b);
automate * concatenation(automate * a, automate * b);
void addEdge(vertex *a,vertex * b, char carac) ;
void affichage(automate autoFiniNonDeter);
automate* motVide();
automate * langagevide(void);
automate* caracter(char *c);
automate * kleen(automate * a);
Enslist * ajouter_Trans(Enslist * e,char car,int voisin);
int vExiste(List * l, int* e, int nb);
Enslist * Derniermayon(List * list);
automate * Determinisation(automate * a);

int main(int nbarg, char *args[])
{

	automate * a=caracter("a");
	automate * b=caracter("b");
	automate * aa=kleen(a);
	affichage(*aa);
    automate * c=concatenation(aa,b);
    affichage(*c);
    automate * d=concatenation(b,aa);
    d->Vertex[1].accepteur=TRUE;
    affichage(*d);
    c=reuninon(c,d);

    affichage(*c);

    c=Determinisation(c);

    affichage(*c);
	return 0;
}

void addEdge(vertex *a,vertex * b, char carac) {

	edge  E;

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

    addEdge(&new->Vertex[0],&new->Vertex[1],c[0]);



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
            addEdge(&new->Vertex[i],&new->Vertex[a->Vertex[i].tab_edge[j].nextVertex->numSommet],a->Vertex[i].tab_edge[j].word);
        }
    }
    decalage=a->nbr_etats;
    new->Vertex[0].accepteur=a->Vertex[0].accepteur | b->Vertex[0].accepteur;

    for (int j = 0; j <b->Vertex[0].nb_edge ; ++j) {
        addEdge(&new->Vertex[0],&new->Vertex[b->Vertex[0].tab_edge[j].nextVertex->numSommet+decalage-1],b->Vertex[0].tab_edge[j].word);
    }

    for (int i = decalage; i <b->nbr_etats+decalage-1 ; i++) {

        new->Vertex[i].numSommet = i;
        new->Vertex[i].accepteur = b->Vertex[i-decalage+1].accepteur;

    }
    for (int i = decalage; i <b->nbr_etats+decalage-1 ; i++) {
        for (int j = 0; j <b->Vertex[i-decalage+1].nb_edge ; j++) {
            addEdge(&new->Vertex[i],&new->Vertex[b->Vertex[i-decalage+1].tab_edge[j].nextVertex->numSommet+decalage-1],b->Vertex[i-decalage+1].tab_edge[j].word);

        }

    }
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
            addEdge(&new->Vertex[i],&new->Vertex[a->Vertex[i].tab_edge[j].nextVertex->numSommet],a->Vertex[i].tab_edge[j].word);
        }
    }
    decalage=a->nbr_etats;

    for (int j = 0; j <a->nbr_etats ; j++) {
        if(new->Vertex[j].accepteur==TRUE){
            new->Vertex[j].accepteur=b->Vertex[0].accepteur;
            for (int k = 0; k <b->Vertex[0].nb_edge ; k++) {
                addEdge(&new->Vertex[j],&new->Vertex[b->Vertex[0].tab_edge[k].nextVertex->numSommet+decalage-1],b->Vertex[0].tab_edge[k].word);
            }
            for (int i = decalage; i <b->nbr_etats+decalage-1 ; i++) {
                new->Vertex[i].numSommet = i;
                new->Vertex[i].accepteur = b->Vertex[i-decalage+1].accepteur;
            }
            for (int i = decalage; i <b->nbr_etats+decalage-1 ; i++) {
                for (int k = 0; k <b->Vertex[i-decalage+1].nb_edge ; k++) {
                    addEdge(&new->Vertex[i],&new->Vertex[b->Vertex[i-decalage+1].tab_edge[k].nextVertex->numSommet+decalage-1],b->Vertex[i-decalage+1].tab_edge[k].word);
                }
            }

            decalage+=b->nbr_etats-1;
        }
    }


    return new;

}



automate * kleen(automate * a){
    a->Vertex[0].accepteur=TRUE;
    for (int  i= 1; i <a->nbr_etats ; i++) {
        if (a->Vertex[i].accepteur == TRUE) {

            for (int j = 0; j < a->Vertex[0].nb_edge; ++j) {
                addEdge(&a->Vertex[i], &a->Vertex[a->Vertex[0].tab_edge[j].nextVertex->numSommet],
                        a->Vertex[0].tab_edge[j].word);
            }
        }
    }
    return a;
}


automate * Determinisation(automate * a){
    List * list=malloc(sizeof(List));
    list->nb=1;

    Enslist * e=malloc(sizeof(Enslist));
    e->First=NULL;
    e->next=NULL;
    e->size=1;
    e->sommet=(int*)malloc(e->size*sizeof(int));
    e->sommet[0]=0;

    list->first=e;
    int etat=0;

    while(etat<list->nb) {

        for (int k = 0; k < e->size; ++k) {

            for (int i = 0; i < a->Vertex[e->sommet[k]].nb_edge; i++) {
                e = ajouter_Trans(e, a->Vertex[e->sommet[k]].tab_edge[i].word,
                                  a->Vertex[e->sommet[k]].tab_edge[i].nextVertex->numSommet);

            }
        }
        Trans * t=e->First;
        for (int j = 0; j <e->nb_T ; ++j) {
            if(vExiste(list,t->sommet,t->size)==-1){
               Enslist * e1=malloc(sizeof(Enslist));
               e1->size=t->size;
               e1->sommet=t->sommet;
               e1->next=NULL;
               e1->First=NULL;
               e1->nb_T=0;
               Derniermayon(list)->next=e1;
               list->nb++;
            }
            t=t->next;
        }
        e=e->next;
        etat++;
    }

    automate * new=malloc(sizeof(automate));
    new->Vertex=(vertex*)malloc(list->nb * sizeof(vertex));
    new->nbr_etats=list->nb;
    e=list->first;

    for (int i = 0; i <list->nb ; ++i) {
        new->Vertex[i].numSommet=i;
        new->Vertex[i].accepteur=FALSE;

        for (int j = 0; j <e->size ; ++j) {
            if(a->Vertex[e->sommet[j]].accepteur==TRUE){
                new->Vertex[i].accepteur=TRUE;
                break;

            }
        }
        Trans * t=e->First;
        for (int j = 0; j <e->nb_T ; ++j) {
            addEdge(&new->Vertex[i],&new->Vertex[vExiste(list,t->sommet,t->size)],t->carra);
            t=t->next;
        }
        e=e->next;
    }

    return new;
}

Enslist * Derniermayon(List * list){
    Enslist * e=list->first;
    while(e->next!=NULL){
        e=e->next;
    }
    return e;
}



int vExiste(List * l, int* e, int nb){
    Enslist  * p;

    p=l->first;

    for (int i = 0; i <l->nb ; i++) {
        if(nb==p->size){
            for (int j = 0; j <nb ; j++) {
                if(e[j]!=p->sommet[j])
                    break;
                else if(j==nb-1 && e[j]==p->sommet[j])
                    return i;
            }
        }

        p=p->next;

    }
    return -1;
}


Enslist * ajouter_Trans(Enslist * e,char car,int voisin){
    Trans * t=malloc(sizeof(Trans));
    Trans * t1=malloc(sizeof(Trans));
    if(e->nb_T==0){
        t->carra=car;
        t->size=1;
        t->sommet=(int*)malloc(sizeof(int));
        t->sommet[0]=voisin;
        t->next=NULL;
        e->First=t;
    }
    else{
        t=e->First;
        while(t->carra!=car && t->next!=NULL){

            t=t->next;
        }
        if(t->carra==car){
           t->size++;
           t->sommet=(int*)realloc(t->sommet, sizeof(int)*t->size);
            t->sommet[t->size-1]=voisin;
            e->nb_T--;
        }
        else{
            t1->carra=car;
            t1->size=1;
            t1->sommet=(int*)malloc(sizeof(int));
            t1->sommet[0]=voisin;
            t1->next=NULL;
            t->next=t1;
        }

    }
    e->nb_T++;
    return e;
}