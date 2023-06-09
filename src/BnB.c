#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include <unistd.h>
#include <time.h>

extern int k;
extern int *d0;
extern int *df;
extern Graph *g;
extern clock_t begin;
extern clock_t end;
int br=0;
int Bwatch[100];
int tdf;
int td0;
Branche LB[1000];

int myComp(const void * v1, const void * v2){
    int fI = * (const int *) v1;
    int sI = * (const int *) v2;
    return nbVoisinv2(g, sI) - nbVoisinv2(g, fI); 
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

int maxIS(int I[][g->nbVertices], int taille){
    int max=0;
    for(int i=0; i<k;i++){
        int temp=listeSize(I[i], taille);
        //printf("taille I : %d\n", temp);
        if(temp>max) max=temp;
    }
    return max;
}

void rvoisins(int sommet, int *listevoisin){
    adjacencyListElement *temp = g->adjacencyLists[sommet];
    while(temp!=NULL){
        listevoisin[temp->v]=1;;
        temp=temp->next;
    }
}

int hop_2_adj(int sommet1, int sommet2, int *ingraph){
    int voisins[g->nbVertices];
    inittab(voisins, g->nbVertices);
    rvoisins(sommet1, voisins);
    adjacencyListElement *temp = g->adjacencyLists[sommet2];
    while(temp!=NULL){
        if(sommet1==temp->v){
            if(!g->branched[sommet2] && g->branched[sommet1])
                return 1;
        }
        else if(ingraph[temp->v] && !g->branched[temp->v] && voisins[temp->v])
            return 1;

        temp=temp->next;
    }
    return 0;
}

void IkNeighbors(Graph* g, int * ISv, int * IS) {
    for(int i=0; i<g->nbVertices; i++){
        if(IS[i]){
            ISv[i]=1;
            Union(ISv, g->adjacencyLists[i]);
        }
    }
}

int score(Graph *g, int * IS, /* int * C, */ int * P, /* int ** S, */ int sommet){
    if(g->branched[sommet]){
        for(int i=0; i<g->nbVertices; i++){
            if(IS[i]){
                if(hop_2_adj(sommet,i,P)) return 0;
            }
        }
        return 1;
    }
    int n1size=0;
    int voisins[g->nbVertices];
    inittab(voisins, g->nbVertices);
    rvoisins(sommet, voisins);
    for(int i=0; i<g->nbVertices; i++)
        if(IS[i] && voisins[i]) n1size++;
    if(n1size) return 1-n1size;
    else if(n1size==0){
        for(int i=0; i<g->nbVertices; i++)
            if(IS[i])
                if(hop_2_adj(sommet,i,P)) return 0;
            
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
        if(g->ingraph[i]){
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
}

Branche ReduceBranches(){
    Branche B;
    /*B.x=g->nbVertices-listeSize(g->branched, g->nbVertices);
    B.B = malloc(B.x * sizeof(int));
    int d=0;
    for(int i=0; i<g->nbVertices; i++) if(!g->branched[i]){
        B.B[d]=i;
        d++;
    }
    return B;*/


/*  int** S = (int**) malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        S[i] = (int*) malloc(g->nbVertices * sizeof(int));
    }
    
    for(int j=0; j<3; j++){
		for(int i=0; i<g->nbVertices;i++){
			S[j][i]=0;
		}
	} */

    /* int **I = (int **) malloc(k * sizeof(int*));
    for(int i=0; i<k; i++){
        I[i] = (int*) malloc((g->nbVertices+1) * sizeof(int*));
    }
 */
    int I[k][g->nbVertices+1];

    for(int j=0; j<k; j++){
		for(int i=0; i<g->nbVertices;i++){
			I[j][i]=0;
		}
        I[j][g->nbVertices]=-1;
	}
    //int C[g->nbVertices];
    int P[g->nbVertices];
    //int U[g->nbVertices];
    for(int i=0; i<g->nbVertices;i++){ 
        //C[i]=0;
        P[i]=0;
        //U[i]=0;
    }
    //setS(g, S, C, U);

    for(int i=g->nbVertices-1; i>-1; i--){
        if(g->ingraph[i] && !g->dom[i]){
            
            /* printf("%d : ", i);
            for(int j=0; j<k; j++){
                printf("\nI%d :", j);
                for(int l=0; l<g->nbVertices; l++) if(I[j][l]) printf(" %d", l);
            }
            getchar(); */
            int scoretot=0;
            int allscore[k];
            P[i]=1;
            for(int j=0; j<k; j++){ 
                allscore[j]=2;
                if(I[j][g->nbVertices]!=-1){
                    /* printf("IS%d\n", j); */
                    allscore[j]=score(g, I[j],/* C, */ P, /* S, */ i);
                    scoretot+=allscore[j];
                }
            }
            int memory=0;
            if(scoretot>=0){
                for(int j=0; j<k; j++){
                    if(allscore[j]!=2){
                        if(allscore[j]==1){
                            memory=1;
                            I[j][i]=1;
                        }
                        else{
                            if(allscore[j]<0) removeConflict(g, I[j], i);
                        }
                    }
                }
                if(!memory){
                    int z=0;
                    while(z<k && allscore[z]!=2) z++;
                    if(z<k){ 
                        I[z][i]=1;
                        I[z][g->nbVertices]=0;
                    }
                    memory=0;
                }
            }
            else P[i]=0;
        }
    }
    int max = maxIS(I, g->nbVertices);
    
    if(max<td0-(tdf+br)){
        /*freeAllS(I, k);
        free(I);
        freeAllS(S, 3);
        free(S); */
        int size=0;
        for(int i=0; i<g->nbVertices; i++){
            if(g->ingraph[i] && !g->branched[i]) size++;
        }
        B.B = malloc(size * sizeof(int));
        B.x = size;
        //printf("size : %d\n", size);
        int c=0;
        for(int i=0; i<g->nbVertices; i++) if(g->ingraph[i] && !g->branched[i]){
            B.B[c]=i;
            c++;
        }
        return B;
    }

    for(int i=g->nbVertices-1; i>-1; i--){
        if(g->ingraph[i] && g->dom[i] && !g->branched[i]){
            int Itemp[k][g->nbVertices];
            for(int j=0; j<k; j++){
                for(int z=0; z<g->nbVertices;z++){
                    Itemp[j][z]=I[j][z];
                }
	        }
            P[i]=1;
            for(int j=0; j<k; j++){ 
                if(I[j][g->nbVertices]!=-1){
                    if(score(g, I[j],  P, i)<0) removeConflict(g, I[j], i);
                }
            }
            int max2 = maxIS(I, g->nbVertices);
            if(max2<td0-(tdf+br)){
                P[i]=0;
                for(int j=0; j<k; j++){
                    for(int z=0; z<g->nbVertices; z++){
                        I[j][z]=Itemp[j][z];
                    }
                }
            }
        }
    }

    /*freeAllS(I, k);
    free(I);
    freeAllS(S, 3);
    free(S); */
    int size=0;
    for(int i=0; i<g->nbVertices; i++){
        if(g->ingraph[i] && !g->branched[i] && !P[i]) size++;
    }
    B.B = malloc(size * sizeof(int));
    B.x = size;
    //printf("size2 : %d\n", size);
    int c=0;
    for(int i=0; i<g->nbVertices; i++) if(g->ingraph[i] && !g->branched[i] && !P[i]){
        B.B[c]=i;
        c++;
    }
    return B;
}

Branche ReduceBranches2(){
    Branche B;
    int C[g->nbVertices];
    for(int i=0; i<g->nbVertices; i++){
        if(!g->branched[i]) C[i]=1;
        else C[i]=0;
    }
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

int * BnB3(){
    if(br==0) for(int i=0; i<100; i++) Bwatch[i]=-1;
    /*printf("B : ");
    int ac=0;
    while(Bwatch[ac]!=-1){
        printf("%d ", Bwatch[ac]);
        ac++;
    }
    printf("\n");
    for(int i=0; i<g->nbVertices; i++) if(df[i]) printf("%d ", i);
    printf("\n");
    afficheDom(g);*/
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
    for(int i=0; i<B.x && listeSize(df, g->nbVertices)>listeSize(d0, g->nbVertices)-1; i++){
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
                for(int j=0; j<g->nbVertices; j++) if(d0[j]){
                    yu++;
                } 
                printf("d0 : %d\n", yu);
            }
            free(temp);
            df[B.B[i]]=0;
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
	iteratif mb[1000];
    tdf=listeSize(df, g->nbVertices), td0=listeSize(d0, g->nbVertices);
	a :
    //afficheDom(g);
    //afficheBranched(g);
    if(tdf+br>=td0){
        //printf("UN\n");
        goto b;
    } 
	if(fullTab(g->dom, g->nbVertices)){
		if(tdf+br<td0){
			for(int i=0; i<g->nbVertices; i++) d0[i]=df[i];
            //for(int i=0; i<g->nbVertices; i++) if(d0[i]) printf("%d ", i);
            td0=tdf+br;
            printf("\nd0 : %d\n", td0);
            end = clock();
            printf("time for upgrade : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);
		}
        //printf("DEUX\n");
		goto b;
    }

	mb[br].last=0;
	mb[br].etage = ReduceBranches();

	if(mb[br].etage.x<=0){
		free(mb[br].etage.B);
        //printf("TROIS\n");
        goto b;
	}
    
	if(mb[br].etage.x>1) qsort(mb[br].etage.B, mb[br].etage.x, sizeof(int), myComp);
	for(int i=mb[br].last; i<mb[br].etage.x && tdf+br<td0-1; i++){
        g->branched[mb[br].etage.B[i]]=1;
        df[mb[br].etage.B[i]]=1;
        domine(mb[br].etage.B[i], g);
        //printf("br %d : %d - ", br,mb[br].etage.B[i]);
		br++;
		goto a;
		b :
		br--;
        if(br==-1) return;
		df[mb[br].etage.B[mb[br].last]]=0;
		unDom(g);
    	domineliste(df, g); 
		i=mb[br].last;
		mb[br].last++;
	}
    for(int i=0; i<mb[br].etage.x; i++) g->branched[mb[br].etage.B[i]]=0;
	free(mb[br].etage.B);
	if(br==0) return;
	else{
        goto b;
       //printf("QUATRE\n");
    }
}