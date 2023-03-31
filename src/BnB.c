#include "../headers/p_center.h"
#include "../headers/misc.h"

#define UNUSED(x) (void)(x)
extern int k;

void removeConflict(Graph *g, adjacencyListElement *IS, int sommet, int number){
    adjacencyListElement *temp=g->adjacencyLists[sommet];
    while(number>0 && temp!=NULL){
        if(inL(IS,temp->v,-1)){
            IS=deleteNode(IS, temp->v);
        }
        temp=temp->next;
    }
}

int maxIS(adjacencyListElement * I[k]){
    int max=0;
    for(int i=0; i<k;i++){
        int temp=listeSize(I[i]);
        if(temp>max) max=temp;
    }
    return max;
}

adjacencyListElement* IkNeighbors(Graph* g, adjacencyListElement* IS) {
    adjacencyListElement* result = NULL;
    result = Union(result, IS);
    adjacencyListElement* temp = IS;
    while (temp != NULL) {
        Union(result, g->adjacencyLists[temp->v]);
        temp = temp->next;
    }
    return result;
}

int score(Graph *g, adjacencyListElement * IS, adjacencyListElement * C, adjacencyListElement * P, adjacencyListElement * S[4], int sommet){
    afficheListe(IS);
    adjacencyListElement * N1 = Intersection(g->adjacencyLists[sommet], IS, -1);
    adjacencyListElement * N2 = Intersection(C, P, -1);
    if(N2!=NULL) N2=Intersection(N2, g->adjacencyLists[sommet], sommet);
    if(N2!=NULL){
        adjacencyListElement * temp = IkNeighbors(g, IS);
        N2=Intersection(N2, temp, -1);
    }
    if(inL(S[0], sommet, -1) && N1==NULL && N2==NULL) return 1;
    if(inL(S[0], sommet, -1) && N1==NULL && N2!=NULL) return 0;
    if((inL(S[0], sommet, -1) || inL(S[1], sommet, -1)) && N1!=NULL) return 1-listeSize(N1);
    if(inL(S[1], sommet, -1) && N1==NULL) return 0;
    if(inL(S[2], sommet, -1) && N2!=NULL) return 0;
    if(inL(S[2], sommet, -1) && N2==NULL) return 1;
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
                if(allscore[i]<0) removeConflict(g, I[i], U->v,allscore[i]);
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
    int max = maxIS(I);
    if(max<listeSize(Dnow)-listeSize(D)) return C;
    else return difference(C, P);
}

adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *U, adjacencyListElement *Dnow){
    afficherGraph(g);
    printf("\nDf : ");
    afficheListe(D);
    printf("U : ");
    afficheListe(U);
    printf("D0 : ");
    afficheListe(Dnow);
    char c =getchar();
    if(U==NULL){
        return D;
    }
    adjacencyListElement *B = ReduceBranches(g,D,U,Dnow);
    if(B==NULL){
        return Dnow;
    }
    
    trierListe(g, B);
    adjacencyListElement *Btemp = B;
    while(Btemp!=NULL){
        adjacencyListElement *U2 = difference(U, g->adjacencyLists[Btemp->v]);
        adjacencyListElement *D2 = D;
        ajoute(&D2,Btemp -> v);
        adjacencyListElement *solo = NULL;
        ajoute(&solo,Btemp -> v);
        U2 = difference(U2, solo);
        printf("ici\n");
        free(solo);
        g -> branched[Btemp->v] = 1;
        adjacencyListElement *newD0 = BnB(g, D2, U2, Dnow);
        afficheListe(newD0);
        if(listeSize(newD0)<listeSize(Dnow)){
            Dnow=NULL;
            Dnow=newD0;
        }
        Btemp = Btemp -> next;
    }
    free(Btemp);
    while(B!=NULL){
        g -> branched[B->v] = 0;
        B= B -> next;
    }
    return Dnow;
}