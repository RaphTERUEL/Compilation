#include "automate.h"
#include <time.h>
int main(int nbarg, char *args[])
{

	automate * a=caracter('a');
	automate * b=caracter('b');

	automate * d=reuninon(a,b);
    d=concatenation(d,a);
    d=concatenation(d,b);

    d=Determinisation(d);
    affichage(*d);
    a=minimisation(d);
    affichage(*a);
    executionautomate(a,"aabaab");
	return 0;
}

void addEdge(vertex *a,vertex * b, char carac) {

	edge  E;
    edge * p;
	a->nb_edge++;
	E.nextVertex=b;
	E.word=carac;
    p=(edge*)realloc(a->tab_edge,sizeof(edge)*a->nb_edge);
    if(!p){
        printf("salut");
    } else{
        a->tab_edge=p;
    }
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
automate* caracter(char c)
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

    addEdge(&new->Vertex[0],&new->Vertex[1],c);



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



automate * minimisation(automate *a){

    int * init=(int*)malloc((a->nbr_etats+1)* sizeof(int));
    int * final=(int*)malloc((a->nbr_etats+1)* sizeof(int));
    int nbtransi=0;
    char * transition=(char *)malloc(sizeof(char));
    int indicetrensition=0;
    int nb=0;
    int indice=0;
    for (int i = 0; i <a->nbr_etats ; ++i) {
        if( a->Vertex[i].accepteur==TRUE)
            final[i]=1;
        else
            final[i]=0;

    }
    final[a->nbr_etats]=0;

    transition=transitiondiferent(a,transition);
    nbtransi= nbtransition(a, transition);
    int ** tab = (int**)malloc(nbtransi* sizeof(int*));
    for (int j = 0; j <nbtransi ; ++j) {
        tab[j]=(int*)malloc((a->nbr_etats+1)* sizeof(int ));
    }


    do{
        for (int k = 0; k <a->nbr_etats+1 ; ++k) {
            init[k]=final[k];
        }

        for (int i = 0; i < a->nbr_etats; ++i) {
            for (int j = 0; j < nbtransi; ++j) {
                indicetrensition= transitionexiste(&a->Vertex[i], transition[j]);
                if(indicetrensition==-1){
                    tab[j][i]=init[a->nbr_etats];

                }
                else{
                    tab[j][i]=init[a->Vertex[i].tab_edge[indicetrensition].nextVertex->numSommet];

                }

            }
        }
        for (int j = 0; j < nbtransi; ++j) {

                tab[j][a->nbr_etats]=init[a->nbr_etats];


        }

        for (int m = 0; m <a->nbr_etats; ++m) {
            final[m]=-1;
        }
        final[0]=0;
        nb=0;

        for (int l = 1; l < a->nbr_etats ; ++l) {
            for (int i = 0; i <l ; ++i) {
                indice=0;
                while(tab[indice][l]==tab[indice][i] && indice<nbtransi-1){
                    indice++;
                }
                if(tab[indice][l]==tab[indice][i] && indice==nbtransi-1){
                    final[l]=final[i];
                    break;
                }

            }
            if(final[l]==-1){
                nb++;
                final[l]=nb;
            }

        }

        nb++;
        final[a->nbr_etats]=nb;
        printf("  \t");
        for (int i = 0; i < a->nbr_etats+1; ++i) {

            printf(" %d ",init[i]);
        }
        printf("\n");
        for (int n = 0; n <nbtransi ; ++n) {
            printf("%c \t",transition[n]);
            for (int i = 0; i < a->nbr_etats+1; ++i) {

                printf(" %d ",tab[n][i]);
            }
            printf("\n");
        }
        printf("  \t");
        for (int i = 0; i < a->nbr_etats+1; ++i) {

            printf(" %d ",final[i]);
        }
        printf("\n\n");
    }while(!tableauegale(init,final,a->nbr_etats+1));



    automate * new=malloc(sizeof(automate));
    new->Vertex=(vertex*)malloc(nb * sizeof(vertex));
    new->nbr_etats=nb;
    indice=0;
    for (int i = 0; indice <nb ; i++) {


        if(final[i]==indice) {

            new->Vertex[indice].numSommet=indice;
            new->Vertex[indice].nb_edge=0;
            new->Vertex[indice].accepteur=a->Vertex[i].accepteur;
            for (int j = 0; j <nbtransi; j++) {
                if(tab[j][i]!=nb){
                    addEdge(&new->Vertex[indice],&new->Vertex[tab[j][i]],transition[j]);
                }
            }
            indice++;
        }
    }



    return new;

}

bool tableauegale(int * a, int * b,int nb){
    for (int i = 0; i <nb ; ++i) {
        if( a[i]!=b[i]){
            return FALSE;
        }
    }
    return TRUE;
}


int transitionexiste(vertex *v, char c){
    for (int i = 0; i <v->nb_edge ; ++i) {
        if(v->tab_edge[i].word==c)
            return i;
    }
    return -1;
}

char *transitiondiferent(automate* a, char *car){
    int nb=a->Vertex[0].nb_edge;
    car=malloc(a->Vertex[0].nb_edge*sizeof(char));
    for (int j = 0; j <a->Vertex[0].nb_edge ; ++j) {
        car[j]=a->Vertex[0].tab_edge[j].word;
    }
    for (int i = 1; i <a->nbr_etats ; ++i) {
        for (int j = 0; j <a->Vertex[i].nb_edge ; ++j) {
            if(charintab(car,a->Vertex[i].tab_edge[j].word,nb)==-1){
                nb++;
                car=realloc(car,nb* sizeof(char));
                car[nb-1]=a->Vertex[i].tab_edge[j].word;
            }
        }

    }


    return car;
}
int nbtransition(automate *a, char *car){
    int nb=0;
    for (int i = 0; i <a->nbr_etats ; ++i) {
        for (int j = 0; j <a->Vertex[i].nb_edge ; ++j) {
            if(charintab(car,a->Vertex[i].tab_edge[j].word,nb)==-1){
                nb++;

            }
        }

    }


    return nb;
}

int charintab(char*car,char c,int nb){
    for (int i = 0; i <nb ; ++i) {
        if(car[i]==c)
            return i;

    }
    return -1;
}


void executionautomate(automate * a, char * mots)
{
    int i=0,existe=-1;
    vertex * courant=&a->Vertex[0];
    printf("%d ",courant->numSommet);
    while(mots[i]!='\0'){

        existe=transitionexiste(courant,mots[i]);
        if(existe!=-1){
            printf("--> %c --> %d ",courant->tab_edge[existe].word,courant->tab_edge[existe].nextVertex->numSommet);
        } else{
            break;
        }
        courant=courant->tab_edge[existe].nextVertex;
        i++;
    }
    if(existe==-1){
        printf("\ncette automate de ne verifie pas le mots");
    } else if(courant->accepteur==TRUE){
        printf("\nExecution terminer avec succès");
    } else if(courant->accepteur==FALSE){
        printf("\nLa dernier letre du mots ne se situe pas sur un etat accepteur");
    }

}