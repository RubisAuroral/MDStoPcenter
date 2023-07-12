#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include <unistd.h>
#include <time.h>

extern int k;
extern int *d0;
extern int *df;
extern clock_t begin;
extern clock_t end;
extern int cuted;
extern int all;
extern int p;
int nb=0;
int level;
int tailledf;
int tailled0;
int backtrack;
Graph *gstate;

int myComp(const void * v1, const void * v2){
    int fI = * (const int *) v1;
    int sI = * (const int *) v2;
    return nbVoisinv2(gstate, sI) - nbVoisinv2(gstate, fI); 
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

int maxIS(int I[][gstate->nbVertices], int taille){
    int max=0;
    for(int i=0; i<k;i++){
        int temp=listeSize(I[i], taille);
        if(temp>max) max=temp;
    }
    return max;
}

void rvoisins(int sommet, int *listevoisin){
    adjacencyListElement *temp = gstate->adjacencyLists[sommet];
    while(temp!=NULL){
        listevoisin[temp->v]=1;;
        temp=temp->next;
    }
}

int hop_2_adj(int sommet1, int sommet2, int *ingraph){
    int voisins[gstate->nbVertices];
    inittab(voisins, gstate->nbVertices);
    rvoisins(sommet1, voisins);
    adjacencyListElement *temp = gstate->adjacencyLists[sommet2];
    while(temp!=NULL){
        if(sommet1==temp->v){
            if(!gstate->branched[sommet2] && gstate->branched[sommet1])
                return 1;
        }
        else if(ingraph[temp->v] && !gstate->branched[temp->v] && voisins[temp->v])
            return 1;

        temp=temp->next;
    }
    return 0;
}

int score(Graph *gstate, int * IS, int * P, int sommet){
    if(gstate->branched[sommet]){
        for(int i=0; i<gstate->nbVertices; i++){
            if(IS[i]){
                if(hop_2_adj(sommet,i,P)) return 0;
            }
        }
        return 1;
    }
    int n1size=0;
    int voisins[gstate->nbVertices];
    inittab(voisins, gstate->nbVertices);
    rvoisins(sommet, voisins);
    for(int i=0; i<gstate->nbVertices; i++)
        if(IS[i] && voisins[i]) n1size++;
    if(n1size) return 1-n1size;
    else if(n1size==0){
        for(int i=0; i<gstate->nbVertices; i++)
            if(IS[i])
                if(hop_2_adj(sommet,i,P)) return 0;
            
        return 1;
    }
    return 0;            
}

void printAllS(Graph *gstate){
    printf("S1 :");
    for(int i=0; i<gstate->nbVertices; i++) if(!gstate->dom[i] && !gstate->branched[i]) printf(" %d", i);
    printf("\nS2 : ");
    for(int i=0; i<gstate->nbVertices; i++) if(gstate->dom[i] && !gstate->branched[i]) printf(" %d", i);
    printf("\nS3 : ");
    for(int i=0; i<gstate->nbVertices; i++) if(!gstate->dom[i] && gstate->branched[i]) printf(" %d", i);
    printf("\nS4 : ");
    for(int i=0; i<gstate->nbVertices; i++) if(gstate->dom[i] && gstate->branched[i]) printf(" %d", i);
    printf("\n");
}

Branche ReduceBranches(){
    Branche B;
    int I[k][gstate->nbVertices+1];
    memset(I, 0, sizeof(I));
    for(int i=0; i<k; i++) I[i][gstate->nbVertices]=-1;
    int P[gstate->nbVertices];
    memset(P, 0, sizeof(P));
    int nbISnotVide=0;
    for(int i=gstate->nbVertices-1; i>-1; i--){
        if(gstate->ingraph[i] && !gstate->dom[i]){
            int scoretot=0;
            int allscore[k];
            for(int j=0; j<k; j++){ 
                allscore[j]=2;
                if(I[j][gstate->nbVertices]!=-1){
                    allscore[j]=score(gstate, I[j], P, i);
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
                            if(allscore[j]<0) removeConflict(gstate, I[j], i);
                        }
                    }
                }
                if(!memory){
                    if(nbISnotVide<k){ 
                        I[nbISnotVide][i]=1;
                        I[nbISnotVide][gstate->nbVertices]=0;
                        nbISnotVide++;
                    }
                    memory=0;
                }
            }
        }
    }
    int max = maxIS(I, gstate->nbVertices);
    
    if(max<tailled0-(tailledf+level)){
        int size=0;
        for(int i=0; i<gstate->nbVertices; i++){
            if(gstate->ingraph[i] && !gstate->branched[i] && nbVoisinv2(gstate, i)!=0) size++;
        }
        B.B = malloc(size * sizeof(int));
        B.x = size;
        int c=0;
        for(int i=0; i<gstate->nbVertices; i++) if(gstate->ingraph[i] && !gstate->branched[i] && nbVoisinv2(gstate, i)!=0){
            B.B[c]=i;
            c++;
        }
        return B;
    }

    for(int i=gstate->nbVertices-1; i>-1; i--){
        if(gstate->ingraph[i] && gstate->dom[i] && !gstate->branched[i]){
            int Itemp[k][gstate->nbVertices+1];
            for(int j=0; j<k; j++){
                for(int z=0; z<gstate->nbVertices+1;z++){
                    Itemp[j][z]=I[j][z];
                }
	        }
            P[i]=1;
            for(int j=0; j<k; j++){ 
                if(I[j][gstate->nbVertices]!=-1){
                    if(score(gstate, I[j],  P, i)<0) removeConflict(gstate, I[j], i);
                }
            }
            max = maxIS(I, gstate->nbVertices);
            if(max<tailled0-(tailledf+level)){
                P[i]=0;
                for(int j=0; j<k; j++){
                    for(int z=0; z<gstate->nbVertices+1; z++){
                        I[j][z]=Itemp[j][z];
                    }
                }
            }
        }
    }

    int size=0, size2=0;
    for(int i=0; i<gstate->nbVertices; i++){
        if(gstate->ingraph[i] && !gstate->branched[i] && nbVoisinv2(gstate, i)!=0){
            size2++;
            if(!P[i]) size++;
        }
    }
    B.B = malloc(size * sizeof(int));
    B.x = size;
    int c=0;
    for(int i=0; i<gstate->nbVertices; i++) if(gstate->ingraph[i] && !gstate->branched[i] && nbVoisinv2(gstate, i)!=0 && !P[i]){
        B.B[c]=i;
        c++;
    }
    return B;
}

Branche ReduceBranches2(){
    Branche B;
    int C[gstate->nbVertices];
    for(int i=0; i<gstate->nbVertices; i++){
        if(gstate->ingraph[i] && !gstate->branched[i]) C[i]=1;
        else C[i]=0;
    }
    int size=listeSize(C, gstate->nbVertices);
        B.B = malloc(size * sizeof(int));
        B.x = size;
        int c=0;
        for(int i=0; i<gstate->nbVertices; i++) if(C[i]){
            B.B[c]=i;
            c++;
        }
        return B;
}

void BnB(Graph *gd){
    level=0, backtrack=0;
    iteratif sauvegarde[1000];
    gstate=gd;
    tailledf=listeSize(df, gstate->nbVertices), tailled0=listeSize(d0, gstate->nbVertices);
    printf("%d-%d\n", p, tailled0);
    while(1){
        for(int i=0; i<gstate->nbVertices; i++){
            if(df[i]) printf("%d ",i);
        }
        printf("\n");
        all++;
        if(tailledf+level>p){
            backtrack=1;
        }            
        
        if(!backtrack && gstate->adom==0){
            end = clock();
            backtrack=1;
            printf("\ntime for upgrade : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);
			for(int i=0; i<gstate->nbVertices; i++) d0[i]=df[i];
            tailled0=tailledf+level;
            printf("d0 : %d\n", tailled0);
            if(tailled0<=gstate->p) return;
            for(int i=0; i<gstate->nbVertices; i++) if(df[i]) printf("%d ", i);
            printf("\n");
        }

	    if(!backtrack){
            Branche maxassign = ReduceBranches2();
            if(maxassign.x>1) qsort(maxassign.B, maxassign.x, sizeof(int), myComp);
            int current = gstate->adom;
            int i;
            for(i=0; current>0 && i<maxassign.x; i++) current -= nbVoisinv2(gstate, maxassign.B[i]);
            free(maxassign.B);
            if(i>=tailled0-tailledf){
                cuted++;
                backtrack=1;
            } 
            else{
                sauvegarde[level].last=0;
                sauvegarde[level].etage = ReduceBranches();

                if(sauvegarde[level].etage.x<=0){
                    free(sauvegarde[level].etage.B);
                    backtrack=1; 
                } 
                
                else{
                    if(sauvegarde[level].etage.x>1) qsort(sauvegarde[level].etage.B, sauvegarde[level].etage.x, sizeof(int), myComp);
                    gstate->branched[sauvegarde[level].etage.B[sauvegarde[level].last]]=1;
                    df[sauvegarde[level].etage.B[sauvegarde[level].last]]=1;
                    domine(sauvegarde[level].etage.B[sauvegarde[level].last], gstate);
                    level++;
                }
            }
        }

        if(backtrack){
            do{
                level--;
                if(level < 0) return;
                
                df[sauvegarde[level].etage.B[sauvegarde[level].last]]=0;
                unDom(gstate);
                domineliste(df, gstate); 
                
                sauvegarde[level].last++;
                
                if (sauvegarde[level].last>sauvegarde[level].etage.x-1){
                    for (int i=0; i<sauvegarde[level].etage.x; i++) {
                        gstate->branched[sauvegarde[level].etage.B[i]]=0;
                    }
                    free(sauvegarde[level].etage.B);
                }
                else break;
            }while (1);
            gstate->branched[sauvegarde[level].etage.B[sauvegarde[level].last]]=1;
            df[sauvegarde[level].etage.B[sauvegarde[level].last]]=1;
            domine(sauvegarde[level].etage.B[sauvegarde[level].last], gstate);
            level++;
        }
        backtrack=0;
    }
}