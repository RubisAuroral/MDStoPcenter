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
int cuted;
int overall;
int nb=0;
int br;
int tdf;
int td0;
Graph *gstate;

int myComp(const void * v1, const void * v2){
    int fI = * (const int *) v1;
    int sI = * (const int *) v2;
    return nbVoisin(gstate, sI) - nbVoisin(gstate, fI); 
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
    
    if(max<td0-(tdf+br)){
        int size=0;
        for(int i=0; i<gstate->nbVertices; i++){
            if(gstate->ingraph[i] && !gstate->branched[i] && nbVoisinv2(gstate, i)!=0) size++;
        }
        B.B = malloc(size * sizeof(int));
        B.x = size;
        overall+=size;
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
            if(max<td0-(tdf+br)){
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
    overall+=size2;
    cuted+=size2-size;
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
        if(!gstate->branched[i]) C[i]=1;
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
    br=0;
    int zlkzed=0;
    iteratif mb[1000];
    gstate=gd;
    tdf=listeSize(df, gstate->nbVertices), td0=listeSize(d0, gstate->nbVertices);
	a :
    end = clock();
    if(((double)(end - begin) / CLOCKS_PER_SEC) >= 540.0){
        printf("time expire\n");
        printf("nombre branche total créées %d, nombre branche coupées %d, nombre de branche réel %d  avec %d appels\n", overall, cuted, overall-cuted, nb);
        exit(-1);
    }

    if(tdf+br>=td0){
        goto b;
    }
    
	if(fullTab(gstate->dom, gstate->nbVertices)){
		if(tdf+br<td0){
            end = clock();
            printf("\ntime for upgrade : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);
			for(int i=0; i<gstate->nbVertices; i++) d0[i]=df[i];
            td0=tdf+br;
            printf("\nd0 : %d\n", td0);
            if(td0<=gstate->p) return;
            for(int i=0; i<gstate->nbVertices; i++) if(df[i]) printf("%d ", i);
		}
		goto b;
    }

	mb[br].last=0;
    nb++;
	mb[br].etage = ReduceBranches();


	if(mb[br].etage.x<=0){
		free(mb[br].etage.B);
        goto b;
	}
    
	if(mb[br].etage.x>1) qsort(mb[br].etage.B, mb[br].etage.x, sizeof(int), myComp);
    if(br==1){
        //printf("%d - ", zlkzed++);
        for(int j=0; j<mb[br].etage.x; j++){ 
            //printf("%d ", mb[br].etage.B[j]);
        }
        //printf("\n");
    }
	for(int i=mb[br].last; i<mb[br].etage.x && tdf+br<td0-1; i++){
        gstate->branched[mb[br].etage.B[i]]=1;
        df[mb[br].etage.B[i]]=1;
        printf("--> branching on %d...\n", mb[br].etage.B[i]);
        domine(mb[br].etage.B[i], gstate);
		br++;
		goto a;
		b :
		br--;
        printf("--> backtrack to level %d...\n", br);
        if(br<0){ 
            return;
        }
		df[mb[br].etage.B[mb[br].last]]=0;
		unDom(gstate);
    	domineliste(df, gstate); 
		i=mb[br].last;
		mb[br].last++;
	}
    for(int i=0; i<mb[br].etage.x; i++) gstate->branched[mb[br].etage.B[i]]=0;
	free(mb[br].etage.B);
	if(br==0){
        return;
        printf("time expire\n");
        printf("nombre branche total créées %d, nombre branche coupées %d, nombre de branche réel %d  avec %d appels\n", overall, cuted, overall-cuted, nb);
    }
	else{
        goto b;
    }
}