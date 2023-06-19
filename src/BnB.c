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

int score(Graph *g, int * IS, int * P, int sommet){
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

void printAllS(Graph *g){
    printf("S1 :");
    for(int i=0; i<g->nbVertices; i++) if(!g->dom[i] && !g->branched[i]) printf(" %d", i);
    printf("\nS2 : ");
    for(int i=0; i<g->nbVertices; i++) if(g->dom[i] && !g->branched[i]) printf(" %d", i);
    printf("\nS3 : ");
    for(int i=0; i<g->nbVertices; i++) if(!g->dom[i] && g->branched[i]) printf(" %d", i);
    printf("\nS4 : ");
    for(int i=0; i<g->nbVertices; i++) if(g->dom[i] && g->branched[i]) printf(" %d", i);
    printf("\n");
}

Branche ReduceBranches(){
    Branche B;
    int I[k][g->nbVertices+1];

    for(int j=0; j<k; j++){
		for(int i=0; i<g->nbVertices;i++){
			I[j][i]=0;
		}
        I[j][g->nbVertices]=-1;
	}
    int P[g->nbVertices];
    for(int i=0; i<g->nbVertices;i++){ 
        P[i]=0;
    }

    for(int i=g->nbVertices-1; i>-1; i--){
        if(g->ingraph[i] && !g->dom[i]){
            
            int scoretot=0;
            int allscore[k];
            for(int j=0; j<k; j++){ 
                allscore[j]=2;
                if(I[j][g->nbVertices]!=-1){
                    allscore[j]=score(g, I[j], P, i);
                    scoretot+=allscore[j];
                }
            }
            int memory=0;
            if(scoretot>=0){
                P[i]=1;
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
        }
    }
    int max = maxIS(I, g->nbVertices);
    
    if(max<td0-(tdf+br)){
        int size=0;
        for(int i=0; i<g->nbVertices; i++){
            if(g->ingraph[i] && !g->branched[i]) size++;
        }
        B.B = malloc(size * sizeof(int));
        B.x = size;
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

    int size=0;
    for(int i=0; i<g->nbVertices; i++){
        if(g->ingraph[i] && !g->branched[i] && !P[i]) size++;
    }
    B.B = malloc(size * sizeof(int));
    B.x = size;
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
        int c=0;
        for(int i=0; i<g->nbVertices; i++) if(C[i]){
            B.B[c]=i;
            c++;
        }
        return B;
}

void BnB(Graph *gd){
	iteratif mb[1000];
    tdf=listeSize(df, gd->nbVertices), td0=listeSize(d0, gd->nbVertices);
	a :

    if(tdf+br>=td0){
        goto b;
    }
    
	if(fullTab(gd->dom, gd->nbVertices)){
		if(tdf+br<td0){
			for(int i=0; i<gd->nbVertices; i++) d0[i]=df[i];
            td0=tdf+br;
            printf("\nd0 : %d\n", td0);
            end = clock();
            printf("time for upgrade : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);
		}
		goto b;
    }

	mb[br].last=0;
	mb[br].etage = ReduceBranches();

	if(mb[br].etage.x<=0){
		free(mb[br].etage.B);
        goto b;
	}
    
	if(mb[br].etage.x>1) qsort(mb[br].etage.B, mb[br].etage.x, sizeof(int), myComp);
	for(int i=mb[br].last; i<mb[br].etage.x && tdf+br<td0-1; i++){
        gd->branched[mb[br].etage.B[i]]=1;
        df[mb[br].etage.B[i]]=1;
        domine(mb[br].etage.B[i], gd);
		br++;
		goto a;
		b :
		br--;
        if(br==-1) return;
		df[mb[br].etage.B[mb[br].last]]=0;
		unDom(gd);
    	domineliste(df, gd); 
		i=mb[br].last;
		mb[br].last++;
	}
    for(int i=0; i<mb[br].etage.x; i++) gd->branched[mb[br].etage.B[i]]=0;
	free(mb[br].etage.B);
	if(br==0) return;
	else{
        goto b;
    }
}