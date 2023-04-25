#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include <unistd.h>

extern int k;
extern int *d0;
extern int *df;
extern Graph *g;
int br=0;
int Bwatch[100];
Branche LB[1000];

int myComp(const void * v1, const void * v2){
    int fI = * (const int *) v1;
    int sI = * (const int *) v2;
    return nbVoisin(g, sI) - nbVoisin(g, fI); 
}

void freeAllS(int **S, int nb){
	for(int i=0; i<nb; i++){
        free(S[i]);
    }
}

void removeConflict(Graph *g, int *IS, int sommet){
    int temp[g->nbVertices];
    for(int i=0; i<g->nbVertices; i++) temp[i]=0;
    Intersection(temp,g->adjacencyLists[sommet], IS);
    int nb=listeSize(temp, g->nbVertices)-1;
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
        //printf("taille I : %d\n", temp);
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
    int save=0;
    int N1v[g->nbVertices];
    int N2v[g->nbVertices];
    int ISv[g->nbVertices];
    for(int i=0; i<g->nbVertices; i++) ISv[i]=0;
    IkNeighbors(g, ISv, IS);
    for(int i=0; i<g->nbVertices; i++){
        N1v[i]=0;
        if(C[i] && P[i] && ISv[i]) N2v[i]=1;
        else N2v[i]=0;
    }
    Intersection(N1v, g->adjacencyLists[sommet], IS);
    if(N2v[sommet]) save=1;
    Intersection(N2v, g->adjacencyLists[sommet], N2v);
    if(save==1) N2v[sommet]=1;
    int N1vn=nullTab(N1v, g->nbVertices), N2vn=nullTab(N2v, g->nbVertices);
    
    if(S[0][sommet] && N1vn && N2vn){ 
        //printf("Score : 1\n");
        return 1;
    }
    if(S[0][sommet] && N1vn && !N2vn){
        //printf("Score : 0\n");
        return 0;
    }
    if((S[0][sommet] || S[1][sommet]) && !N1vn){
        return 1-listeSize(N1v, g->nbVertices);
    }
    if(S[1][sommet] && N1vn){
        //printf("Score : 0\n");
        return 0;
    }
    if(S[2][sommet] && !N2vn){
        //printf("Score : 0\n");
        return 0;
    }
    if(S[2][sommet] && N2vn){
        //printf("Score : 1\n");
        return 1;
    }
    return 0;
}

void printAllS(Graph *g, int ** S){
    printf("S1 :");
    for(int i=0; i<g->nbVertices; i++) if(S[0][i]) printf(" %d", i);
    printf("\nS2 : ");
    for(int i=0; i<g->nbVertices; i++) if(S[1][i]) printf(" %d", i);
    printf("\nS3 : ");
    for(int i=0; i<g->nbVertices; i++) if(S[2][i]) printf(" %d", i);
    printf("\nS4 : ");
    for(int i=0; i<g->nbVertices; i++) if(g->dom[i] && g->branched[i]) printf(" %d", i);
    printf("\n");
}

void setS(Graph *g, int **S, int *C, int *U){
    for(int i=0; i<g->nbVertices; i++){
        if(g->dom[i]){
            if(!g->branched[i]){
                S[1][i]=1;
                C[i]=1;
            }
        }
        else{
            U[i]=1;
            if(g->branched[i]){
                S[2][i]=1;
            } 
            else{
                C[i]=1;
                S[0][i]=1;
            }
        }
    }
}

Branche ReduceBranches(){
    //printf("k : %d\n", k);
    Branche B;
    /*B.x=g->nbVertices-listeSize(g->branched, g->nbVertices);
    B.B = malloc(B.x * sizeof(int));
    int d=0;
    for(int i=0; i<g->nbVertices; i++) if(!g->branched[i]){
        B.B[d]=i;
        d++;
    }
    return B;*/
    int** S = (int**) malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        S[i] = (int*) malloc(g->nbVertices * sizeof(int));
    }
    
    for(int j=0; j<3; j++){
		for(int i=0; i<g->nbVertices;i++){
			S[j][i]=0;
		}
	}

    int C[g->nbVertices];
    int P[g->nbVertices];
    int U[g->nbVertices];
    for(int i=0; i<g->nbVertices;i++){ 
        C[i]=0;
        P[i]=0;
        U[i]=0;
    }
    setS(g,S,C,U);
    
    //printAllS(g,S);
    int **I = (int **) malloc(k * sizeof(int*));
    for(int i=0; i<k; i++){
        I[i] = (int*) malloc(g->nbVertices * sizeof(int*));
    }

    for(int j=0; j<k; j++){
		for(int i=0; i<g->nbVertices;i++){
			I[j][i]=0;
		}
	}

    for(int i=g->nbVertices-1; i>-1; i--){
        if(U[i]){
            int scoretot=0;
            
            for(int j=0; j<k; j++){ 
                if(!nullTab(I[j], g->nbVertices)){
                    scoretot=score(g, I[j],C, P, S, i);
                }
            }
            //printf("\n");
            if(scoretot>=0){
                P[i]=1;
                for(int j=0; j<k; j++){
                    if(!nullTab(I[j], g->nbVertices)){
                        if(score(g, I[j],C, P, S, i)>0){
                            //printf("I%d\n", j);
                            I[j][i]=1;
                        }
                        else{
                            if(score(g, I[j],C, P, S, i)<0) removeConflict(g, I[j], i);
                        }
                    }
                    else{
                        int z=0;
                        while(z<k && !nullTab(I[z], g->nbVertices)) z++;
                        if(z<k) I[z][i]=1;
                    }
                }
            }
        }
    }
    
    int max = maxIS(I, g->nbVertices);
    //printf("max : %d - ", max);
    if(max<listeSize(d0,g->nbVertices)-listeSize(df, g->nbVertices)){
        freeAllS(I, k);
        free(I);
        freeAllS(S, 3);
        free(S);
        int size=listeSize(C, g->nbVertices);
        B.B = malloc(size * sizeof(int));
        B.x = size;
        //printf("size : %d\n", size);
        int c=0;
        for(int i=0; i<g->nbVertices; i++) if(C[i]){
            B.B[c]=i;
            c++;
        }
        return B;
    }
    for(int i=g->nbVertices-1; i>-1; i--){
        if(S[1][i]){
            int **Itemp = (int **) malloc(k * sizeof(int*));
            for(int j=0; j<k; j++){
                Itemp[j] = (int*) malloc(g->nbVertices * sizeof(int*));
            }
            for(int j=0; j<k; j++){
                for(int z=0; z<g->nbVertices;z++){
                    Itemp[j][z]=0;
                }
	        }
            P[i]=1;
            for(int j=0; j<k; j++){ 
                if(!nullTab(I[j], g->nbVertices)){
                    if(score(g, I[j],C, P, S, i)<0) removeConflict(g, I[j], i);
                }
            }
            int max2 = maxIS(I, g->nbVertices);
            if(max2<listeSize(d0,g->nbVertices)-listeSize(df, g->nbVertices)){
                P[i]=0;
                for(int j=0; j<k; j++){
                    for(int z=0; z<g->nbVertices; z++){
                        I[j][z]=Itemp[j][z];
                    }
                }
            }
            freeAllS(Itemp, k);
            free(Itemp);
        }
    }
    freeAllS(I, k);
    free(I);
    freeAllS(S, 3);
    free(S);
    for(int i=0; i<g->nbVertices; i++) if(P[i]) C[i]=0;
    int size=listeSize(C, g->nbVertices);
    B.B = malloc(size * sizeof(int));
    B.x = size;
    //printf("size2 : %d\n", size);
    int c=0;
    for(int i=0; i<g->nbVertices; i++) if(C[i]){
        B.B[c]=i;
        c++;
    }
    return B;
}

int * BnB3(){
    if(br==0) for(int i=0; i<100; i++) Bwatch[i]=-1;
    printf("B : ");
    int ac=0;
    while(Bwatch[ac]!=-1){
        printf("%d ", Bwatch[ac]);
        ac++;
    }
    printf("\n");
    //for(int i=0; i<g->nbVertices; i++) printf("%d", g->branched[i]);
    //printf("\n");  
    if(fullTab(g->dom, g->nbVertices)){
        int * aenv = malloc(g->nbVertices * sizeof(int));
        for(int i=0; i<g->nbVertices; i++) aenv[i]=df[i];
        return aenv;
    }

    Branche B = ReduceBranches();
    if(B.x==0){
        int * aenv = malloc(g->nbVertices * sizeof(int));
        for(int i=0; i<g->nbVertices; i++) aenv[i]=d0[i];
        free(B.B);
        return aenv;
    }
    if(B.x>1) qsort(B.B, B.x, sizeof(int), myComp);
    for(int i=0; i<B.x && listeSize(d0, g->nbVertices)-listeSize(df, g->nbVertices)>=1; i++){
        unDom(g);
        domineliste(df, g); 
        if((nbVoisin(g, B.B[i])>0 || !g->dom[B.B[i]]) && !g->branched[B.B[i]]){
            g->branched[B.B[i]]=1;
            df[B.B[i]]=1;
            domine(B.B[i], g);
            Bwatch[br]=B.B[i];
            br++;
            int * temp = BnB3();
            br--;
            Bwatch[br]=-1;
            if(listeSize(temp, g->nbVertices)<listeSize(d0, g->nbVertices)){
                int yu=0;
                for(int j=0; j<g->nbVertices; j++) d0[j]=temp[j];
                for(int j=0; j<g->nbVertices; j++) if(d0[j]) yu++;
                printf("d0 : %d\n", yu);
            }
            free(temp);
            df[B.B[i]]=0;
        }
    }
    unDom(g);
    domineliste(df, g); 
    for(int i=0; i<B.x; i++) g->branched[B.B[i]]=0;
    int * aenv = malloc(g->nbVertices * sizeof(int));
    for(int i=0; i<g->nbVertices; i++) aenv[i]=d0[i];
    free(B.B);
    return aenv;
}

void BnBtest(){
	iteratif mb[100];
	a :
	if(fullTab(g->dom, g->nbVertices)){
		if(listeSize(df, g->nbVertices)<listeSize(d0, g->nbVertices)){
			for(int i=0; i<g->nbVertices; i++) d0[i]=df[i];
            for(int i=0; i<g->nbVertices; i++) if(d0[i]) printf("%d ", i);
            printf("\nd0 : %d\n", listeSize(d0, g->nbVertices));
		}
		goto b;
    }

	mb[br].last=0;
	mb[br].etage = ReduceBranches();
	if(mb[br].etage.x==0){
		goto b;
	}
    
	if(mb[br].etage.x>1) qsort(mb[br].etage.B, mb[br].etage.x, sizeof(int), myComp);
	for(int i=mb[br].last; i<mb[br].etage.x; i++){
		g->branched[mb[br].etage.B[i]]=1;
        df[mb[br].etage.B[i]]=1;
        domine(mb[br].etage.B[i], g);
		br++;
		goto a;
		b :
		br--;
		df[mb[br].etage.B[mb[br].last]]=0;
		unDom(g);
    	domineliste(df, g); 
		i=mb[br].last;
		mb[br].last++;
	}
    for(int i=0; i<mb[br].etage.x; i++) g->branched[mb[br].etage.B[i]]=0;
	free(mb[br].etage.B);
	if(br==0) return;
	else goto b;
}