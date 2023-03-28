#include "../headers/p_center.h"
#include "../headers/misc.h"

adjacencyListElement *ReduceBranches(Graph *g, adjacencyListElement *D, adjacencyListElement *U, adjacencyListElement *Dnow){
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
    for(int i=0; i<listeSize(B); i++){
        adjacencyListElement *U2 = difference(U, g->adjacencyLists[B->v]);
        adjacencyListElement *solo = NULL;
        solo->v=B->v;
        U2 = difference(U, solo);
        free(solo);
        //mark i as branched
        //test
    }
    printf("b\n");
}