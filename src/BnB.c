#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"

#define UNUSED(x) (void)(x)
extern int k;
extern adjacencyListElement *d0;
extern adjacencyListElement *df;

adjacencyListElement * removeConflict(Graph *g, adjacencyListElement *IS, int sommet){
    adjacencyListElement *temp=Intersection(g->adjacencyLists[sommet], IS,-1);
    int nb =listeSize(temp)-1;
    while(nb>0 && temp!=NULL){
        IS=deleteNode(IS, temp->v);
        temp=temp->next;
        nb--;
    }
    return IS;
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
    adjacencyListElement * N1 = Intersection(g->adjacencyLists[sommet], IS, -1);
    adjacencyListElement * N2 = Intersection(C, P, -1);
    if(N2!=NULL) N2=Intersection(N2, g->adjacencyLists[sommet], sommet);
    if(N2!=NULL){
        adjacencyListElement * temp = IkNeighbors(g, IS);
        N2=Intersection(N2, temp, -1);
        freeList(temp);
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

void setS(Graph *g, adjacencyListElement * S[4]){
    for(int i=0; i<g->nbVertices; i++){
        if(listeSize(g->adjacencyLists[i])!=0){
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

adjacencyListElement *ReduceBranches(Graph *g, adjacencyListElement *U){
    //printf("k : %d\n", k);
    adjacencyListElement * P = NULL;
    
    adjacencyListElement * S[4];
    for(int i=0; i<4; i++) S[i]=NULL;
    
    adjacencyListElement * I[k];
    for(int i=0; i<k; i++) I[i]=NULL;

    adjacencyListElement * C = NULL;
    //afficheDom(g);
    setS(g, S);
    printAllS(S);
    /*
    C=Union(C, S[0]);
    C=Union(C, S[1]);
    freeList(P);
    for(int i=0; i<4; i++){ 
        freeList(S[i]);
    }
    for(int i=0; i<k; i++){ 
        freeList(I[i]);
    }
    afficheListe(C);
    return C;
    //printf("C : ");
    //afficheListe(C);
    
    while(U!=NULL){
        int scoretot=0;
        int memory=0;
        int allscore[k];
        for(int i=0; i<k; i++) allscore[i]=2;
        
        for(int i=0; i<k; i++){ 
            if(I[i]!=NULL){
                allscore[i]=score(g, I[i],C,P, S, U->v);
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
                if(allscore[i]<0) I[i]=removeConflict(g, I[i], U->v);
            }
            if(memory==0){
                int j=0;
                while(I[j]!=NULL && j<k) j++;
                if(j<k) ajoute(&I[j], U->v);
            }
        }
        U=U->next;
    }
    int max = maxIS(I);
    if(max<listeSize(d0)-listeSize(df)){
        return C;
    }
    
    
    /*adjacencyListElement * Swd = NULL;
    printf("\nS[1] : ");
    afficheListe(S[1]);
    Swd=Union(Swd, S[1]);
    
    while(Swd!=NULL){
        ajoute(&P, Swd->v);
        adjacencyListElement * Itemp[k];
        for(int j=0; j<k; j++){
            Itemp[j]=NULL;
            Itemp[j]=Union(Itemp[j], I[j]);
        }
        
        for(int i=0; i<k; i++){
            printf("IS%d : ",i);
            afficheListe(I[i]);
        }

        for(int j=0; j<k; j++){
            if(I[j]!=NULL){
                if(score(g, I[j],C,P, S, Swd->v)<0){
                    //getchar();
                    afficheListe(I[j]);
                    I[j]=removeConflict(g,I[j],Swd->v);
                    afficheListe(I[j]);
                    //getchar();
                }
            }
        }
        
        for(int i=0; i<k; i++){
            printf("IS%d : ",i);
            afficheListe(I[i]);
        }

        if(maxIS(I)<listeSize(d0)-listeSize(df)){
            printf("vrai ?");
            for(int j=0; j<k; j++){
                I[j]=Union(I[j], Itemp[j]);
            }
            deleteNode(P, Swd->v);
        }
        Swd=Swd->next;
    }*/

    //return difference(C, P);
}

/*adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *Dnow){
    unDom(g);
    domineliste(D, g);
    adjacencyListElement *U = undomlist(g);
    printf("\nDf : ");
    afficheListe(D);
    printf("U : ");
    afficheListe(U);
    printf("D0 : ");
    afficheListe(Dnow);
    if(U==NULL){
        return D;afficheListe(S[2]);
    afficheListe(B);
    adjacencyListElement *Btemp = B;
    while(Btemp!=NULL){
        //getchar();
        printf("B restant : ");
        afficheListe(Btemp);
        if(listeSize(D)<listeSize(Dnow)){
            g -> branched[Btemp->v] = 1;
            adjacencyListElement *U2 = difference(U, g->adjacencyLists[Btemp->v]);
            ajoute(&D,Btemp -> v);
            adjacencyListElement *solo = NULL;
            ajoute(&solo,Btemp -> v);
            U2 = difference(U2, solo);
            freeList(solo);
            adjacencyListElement *newD0 = BnB(g, D, Dnow);
            if(listeSize(newD0)<listeSize(Dnow)){
                afficheListe(newD0);    
                Dnow->v = newD0->v;
                Dnow->next=newD0->next;
            }
            D = deleteNode(D, Btemp->v);
            printf("Sauvetage cliché : ");
            afficheListe(D);
            unDom(g);
            domineliste(D, g);
            freeList(U2);
        }  afficheListe(new);
        Btemp = Btemp -> next;
    }
    
    freeList(Btemp);
    
    Btemp = B;
    while(Btemp!=NULL){
        g -> branched[Btemp->v] = 0;
        Btemp = Btemp -> next;
    }
    return Dnow;for(int j=0; j<k; j++){
            Itemp[j]=NULL;
}*/

adjacencyListElement * BnB2(Graph *g){
    unDom(g);
    domineliste(df, g);
    adjacencyListElement *U = undomlist(g);
    printf("\nDf (%d): ", listeSize(df));
    afficheListe(df);
    /*printf("U : ");
    afficheListe(U);*/
    printf("\nD0 (%d): ", listeSize(d0));
    afficheListe(d0);

    if(U==NULL){
        freeList(U);
        return df;
    }
    adjacencyListElement *B = ReduceBranches(g,U);
    /*
    if(B==NULL){
        freeList(U);
        freeList(B);
        return d0;
    }
    
    trierListe(g, B);
    adjacencyListElement *Btemp = NULL;
    if(listeSize(df)<listeSize(d0)){
        while(B!=NULL){
            ajoute(&Btemp, B->v);
            g -> branched[B->v] = 1;
            ajoute(&df,B -> v);
            adjacencyListElement *solo = NULL;
            ajoute(&solo,B -> v);
            freeList(solo);
            adjacencyListElement *newD0 = BnB2(g);
            if(listeSize(newD0)<listeSize(d0)){  
                d0->v = newD0->v;
                d0->next=newD0->next;
            }
            df = deleteNode(df, B->v);
            B = B -> next;
        }
    }
    afficheListe(Btemp);
    while(Btemp!=NULL){
        printf("valeur du crash : %d\n", Btemp->v);
        g -> branched[Btemp->v] = 0;
        Btemp = Btemp -> next;
    }
    freeList(U);
    freeList(B);
    freeList(Btemp);
    return d0;*/
}