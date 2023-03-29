#include "../headers/p_center.h"
#include "../headers/misc.h"

#define UNUSED(x) (void)(x)
extern int k;

void printAllS(adjacencyListElement * S[4]){
    printf("S1 : ");
    afficheListe(S[0]);
    printf("S2 : ");
    afficheListe(S[1]);
    printf("S3 : ");
    afficheListe(S[2]);
    printf("S4 : ");
    afficheListe(S[3]);
}

void setS(Graph *g, adjacencyListElement * S[4], adjacencyListElement *Df){
    for(int i=0; i<g->nbVertices; i++){
        if(inL(Df, i, -1)==0 && listeSize(g->adjacencyLists[i])!=0){
            switch(g->dom[i]){
                case 1:
                    switch(g->branched[i]){
                        case 1: ajoute(&S[3], i); break;
                        case 0: ajoute(&S[1], i); break;
                    }
                    break;
                case 0:
                    switch(g->branched[i]){
                        case 1: ajoute(&S[2], i); break;
                        case 0: ajoute(&S[0], i); break;
                    }
                    break;
            }
        }
    }
}

adjacencyListElement *ReduceBranches(Graph *g, adjacencyListElement *D, adjacencyListElement *U, adjacencyListElement *Dnow){
    UNUSED(g);
    UNUSED(D);
    UNUSED(U);
    UNUSED(Dnow);
    printf("k : %d\n", k);
    adjacencyListElement * P = NULL;
    
    adjacencyListElement * S[4];
    for(int i=0; i<4; i++) S[i]=NULL;
    
    adjacencyListElement * I[k];
    for(int i=0; i<k; i++) I[i]=NULL;

    adjacencyListElement * C = NULL; 
    setS(g, S, D);
    printAllS(S);
    addListeToListe(&C, S[0]);
    addListeToListe(&C, S[1]);
    printf("C : ");
    afficheListe(C);
    return NULL;
}

adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *U, adjacencyListElement *Dnow){
    afficherGraph(g);
    printf("\nDf : ");
    afficheListe(D);
    printf("U : ");
    afficheListe(U);
    printf("D0 : ");
    afficheListe(Dnow);

    if(U==NULL){
        return Dnow;
    }
    adjacencyListElement *B = ReduceBranches(g,D,U,Dnow);
    if(B==NULL){
        return Dnow;
    }
    //trier B
    adjacencyListElement *Btemp = B;
    while(Btemp!=NULL){
        adjacencyListElement *U2 = difference(U, g->adjacencyLists[Btemp->v]);
        adjacencyListElement *solo = NULL;
        solo -> v = Btemp -> v;
        U2 = difference(U, solo);
        free(solo);
        g -> branched[Btemp->v] = 1;
        adjacencyListElement *newD0 = BnB(g, D, U2, Dnow);
        if(listeSize(newD0)<listeSize(Dnow)){
            Dnow=newD0;
        }
        Btemp = Btemp -> next;
    }
    free(Btemp);
    while(B!=NULL){
        g -> branched[B->v] = 1;
        B= B -> next;
    }
    printf("b\n");
    return NULL;
}