#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"

extern int k;
extern int *d0;
extern int *df;
int br=-1;

void freeAllS(int **S, int nb){
	for(int i=0; i<nb; i++){
        free(S[i]);
    }
}

void removeConflict(Graph *g, int *IS, int sommet){
    int temp[g->nbVertices];
    for(int i=0; i<g->nbVertices; i++) temp[i]=0;
    Intersection(temp,g->adjacencyLists[sommet], IS,g->nbVertices);
    int nb =listeSize(temp, g->nbVertices)-1;
    for(int i=0; i<g->nbVertices && nb>0; i++){
        if(temp[i]){
            IS[i]=0;
            nb--;
        }
    }
}

int maxIS(int ** I, int taille){
    int max=0;
    for(int i=0; i<k;i++){
        int temp=listeSize(I[i], taille);
        if(temp>max) max=temp;
    }
    return max;
}

void IkNeighbors(Graph* g, int * ISv, int * IS) {
    for(int i=0; i<g->nbVertices; i++){ 
        if(IS[i]){ 
            ISv[i]=1;
            Union(ISv, g->adjacencyLists[i]);
        }
    }
}

int score(Graph *g, int * IS, int * C, int * P, int ** S, int sommet){
    int N1[g->nbVertices];
    int N2[g->nbVertices];
    int ISv[g->nbVertices];
    for(int i=0; i<g->nbVertices; i++) ISv[i]=0;
    IkNeighbors(g, ISv, IS);
    for(int i=0; i<g->nbVertices; i++){
        N1[i]=0;
        if(C[i] && P[i] && ISv[i]) N2[i]=1;
        else N2[i]=0;
    }
    Intersection(N1, g->adjacencyLists[sommet], IS, g->nbVertices);
    Intersection(N2, g->adjacencyLists[sommet], N2, g->nbVertices);
    int N1n=nullTab(N1, g->nbVertices), N2n=nullTab(N2, g->nbVertices);
    if(S[0][sommet] && N1n && N2n) return 1;
    if(S[0][sommet] && N1n && !N2n) return 0;
    if((S[0][sommet] || S[1][sommet]) && !N1n) return 1-listeSize(N1, g->nbVertices);
    if(S[1][sommet] && N1n) return 0;
    if(S[2][sommet] && !N2n) return 0;
    if(S[2][sommet] && N2n) return 1;
}

void printAllS(Graph *g, int ** S){
    printf("S1 :");
    for(int i=0; i<g->nbVertices; i++) if(S[0][i]) printf(" %d", i);
    printf("\nS2 : ");
    for(int i=0; i<g->nbVertices; i++) if(S[1][i]) printf(" %d", i);
    printf("\nS3 : ");
    for(int i=0; i<g->nbVertices; i++) if(S[2][i]) printf(" %d", i);
}

void setS(Graph *g, int **S, int *C){
    for(int i=0; i<g->nbVertices; i++){
        if(g->adjacencyLists[i]!=NULL){
            if(g->dom[i]){
                if(!g->branched[i]){
                    S[1][i]=1;
                    C[i]=1;
                }
            }
            else{
                if(g->branched[i]) S[2][i]=1;
                else{
                    C[i]=1;
                    S[0][i]=1;
                }
            }
        }
    }
}

void ReduceBranches(Graph *g, Branche * B){
    //printf("k : %d\n", k);
    
    int** S = (int**) malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        S[i] = (int*) malloc(g->nbVertices * sizeof(int));
    }
    
    for(int j=0; j<3; j++){
		for(int i=0; i<g->nbVertices;i++){
			S[j][i]=0;
			S[j][i]=0;
			S[j][i]=0;
		}
	}
    int C[g->nbVertices];
    int P[g->nbVertices];
    for(int i=0; i<g->nbVertices;i++){ 
        C[i]=0;
        P[i]=0;
    }
    setS(g,S,C);
    printAllS(g,S);
    int **I = (int **) malloc(k * sizeof(int*));
    for(int i=0; i<k; i++){
        I[i] = (int*) malloc(g->nbVertices * sizeof(int*));
    }

    for(int j=0; j<k; j++){
		for(int i=0; i<g->nbVertices;i++){
			I[j][i]=0;
			I[j][i]=0;
			I[j][i]=0;
		}
	}

    for(int i=0; i<g->nbVertices; i++){
        if(g->dom[i]==0){
            int scoretot=0;
            int memory=0;
            int allscore[k];
            for(int j=0; j<k; j++) allscore[i]=2;
            
            for(int j=0; j<k; j++){ 
                if(!nullTab(I[j], g->nbVertices)){
                    allscore[j]=score(g, I[i],C, P, S, i);
                    scoretot+=allscore[i];
                }
            }
            if(scoretot>=0){
                P[i]=1;
                for(int j=0; j<k; j++){ 
                    if(allscore[j]!=2 && allscore[j]>0){
                        I[j][i];
                        memory=1;
                    }
                    if(allscore[j]<0) removeConflict(g, I[j], i);
                }
                if(memory==0){
                    int z=0;
                    while(!nullTab(I[z], g->nbVertices) && z<k) z++;
                    if(z<k) I[z][i]=1;
                }
            }
        }
    }
    int max = maxIS(I, g->nbVertices);
    if(max<listeSize(d0,g->nbVertices)-listeSize(df, g->nbVertices)){
        int size=listeSize(C, g->nbVertices);
        printf("size : %d\n", size);
        B[br].B = malloc(size * sizeof(int));
        B[br].x = size;
        int c=0;
        for(int i=0; i<size; i++) if(C[i]){
            B[br].B[c]=i;
            c++;
        }
        return;
    }
    freeAllS(I, k);
    free(I);
    freeAllS(S, 3);
    free(S);
    for(int i=0; i<g->nbVertices; i++) if(C[i] && P[i]) C[i]=0;
    int size=listeSize(C, g->nbVertices);
    printf("size : %d\n", size);
    B[br].B = malloc(size * sizeof(int));
    B[br].x = size;
    int c=0;
    for(int i=0; i<g->nbVertices; i++) if(C[i]){
        B[br].B[c]=i;
        c++;
    }
    return;
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
    return NULL;
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
        return D;
    }
    adjacencyListElement *B = ReduceBranches(g,D,U,Dnow);
    if(B==NULL){
        return Dnow;
    }
    trierListe(g, B);
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
    br++;
    unDom(g);
    domineliste(df, g);
    afficheDom(g);

    if(fullTab(g->dom, g->nbVertices)){
        return df;
    }
    
    Branche LB[g->nbVertices-listeSize(df, g->nbVertices)];
    ReduceBranches(g, LB);
    for(int i=0; i<LB[br].x; i++)  printf("%d\n", LB[br].B[i]);
    if(nullTab(LB[br].B, g->nbVertices)){
        return d0;
    } 
    /*
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
    freeList(Btemp);
    return d0;*/
    //free(B);
    return NULL;
}