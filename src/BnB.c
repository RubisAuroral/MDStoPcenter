#include "../headers/p_center.h"
#include "../headers/misc.h"

#define UNUSED(x) (void)(x)
extern int k;



int score(Graph *g, adjacencyListElement * IS, adjacencyListElement * C, adjacencyListElement * P, adjacencyListElement * S[4], int sommet){
    afficheListe(IS);
    adjacencyListElement * N1 = Intersection(g->adjacencyLists[sommet], IS, -1);
    adjacencyListElement * N2 = Intersection(C, P, -1);
    if(N2!=NULL) N2=Intersection(N2, g->adjacencyLists[sommet], sommet);
    return 0;
}

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
    C=Union(C, S[0]);
    C=Union(C, S[1]);
    printf("C : ");
    afficheListe(C);
    
    while(U!=NULL){
        int scoretot=0;
        int memory=0;
        int allscore[k];
        for(int i=0; i<k; i++) allscore[i]=2;
        
        for(int i=0; i<k; i++){ 
            if(I[i]!=NULL){
                allscore[i]=score(g, I[0],C,P, S, U->v);
                scoretot+=allscore[i];
            }
        }

        if(scoretot>=0){
            ajoute(&P, U->v);
            for(int i=0; i<k; i++){ 
                if(allscore[i]!=2 && allscore[i]>0){
                    ajoute(&I[i], U->v);
                    memory=1;
                }
                if(allscore[i]<0); //c'est chiant
            }
            if(!memory){
                int j=0;
                while(I[j]!=NULL && j<k) j++;
                if(j<k) ajoute(&I[j], U->v);
            }
        }
        U=U->next;
    }
    for(int i=0; i<k; i++){
        if(I[i]!=NULL){
            printf("IS%d : ", i);
            afficheListe(I[i]);
        }
    }
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