#include "../headers/p_center.h"
#include "../headers/misc.h"

int bestToChoose(Graph *gd){
	int r,zb=0, b=0;
	for (int i = 0; i < gd->nbVertices; i++){
		r=0;
		if(gd -> dom[i] == 0) r++;
	    adjacencyListElement *adj = gd->adjacencyLists[i];
	    while (adj != NULL){
	        if(gd->dom[adj->v] == 0) r++;
	        adj = adj->next;
	    }
	    if(r>=zb){
			zb=r; 
			b=i;
		}
	}
	return b;
}

int undomBy(Graph *g, int x){
	int count=0;
	adjacencyListElement *temp = g->adjacencyLists[x];
	while(temp!=NULL){
		if(g->dom[temp->v]==0) count++;
		temp=temp->next;
	}
	return count;
}

void domine(int x, Graph *gd){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){
	    gd->dom[adj->v]=1;
	    adj = adj->next;
	}
	gd->dom[x]=1;
}

void domineliste(int *sol, Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(sol[i]==1 || g->adjacencyLists[i]==NULL){
			domine(i, g);
		}
	}
}

int count(Graph *gd, int x){
	for(int i=0; i<x; i++){
		if(gd->dom[i]==0) return 0;
	}
	return 1;
}

void afficheDom(Graph *gd){
	for(int i=0; i<gd->nbVertices; i++) printf("%d", gd->dom[i]);
	printf("\n");
}

void afficheBranched(Graph *gd){
	for(int i=0; i<gd->nbVertices; i++) printf("%d", gd->branched[i]);
	printf("\n");
}

void unDom(Graph *g){
	for(int i=0; i<g->nbVertices;i++) g->dom[i]=0;
}

void createN1(Graph *gd, int x, int *N1){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	int adjtab[gd->nbVertices];
	for(int i=0; i<gd->nbVertices; i++) adjtab[i]=0;
	while (adj != NULL){
		adjtab[adj->v]=1;
		adj=adj->next;
	}
	for(int i=0; i<gd->nbVertices; i++){
		if(adjtab[i]){
	        adjacencyListElement *adj2 = gd->adjacencyLists[i];
	        int adjtab2[gd->nbVertices];
			for(int j=0; j<gd->nbVertices; j++) adjtab2[j]=0;
			while (adj2 != NULL){
				adjtab2[adj2->v]=1;
				adj2=adj2->next;
			}
	        for(int j=0; j<gd->nbVertices; j++) if(adjtab2[j] && !adjtab[j] && j!=x){
				N1[i]=1;
				break;
			}
		}
	}
}

void createN2(Graph *gd, int x, int *N1, int *N2){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	int adjtab[gd->nbVertices];
	for(int i=0; i<gd->nbVertices; i++) adjtab[i]=0;
	while (adj != NULL){
		adjtab[adj->v]=1;
		adj=adj->next;
	}
	
	for(int i=0; i<gd->nbVertices; i++){
		if(adjtab[i]){
	        if(N1[i]==0){
	        	adjacencyListElement *adj2 = gd->adjacencyLists[i];
				int adjtab2[gd->nbVertices];
				for(int j=0; j<gd->nbVertices; j++) adjtab2[j]=0;
				while (adj2 != NULL){
					adjtab2[adj2->v]=1;
					adj2=adj2->next;
				}
	        	for(int j=0; j<gd->nbVertices; j++) if(adjtab2[j] && N1[j]){
					N2[i]=1;
					break;
				} 
	        } 
		}
	}
}

void createN3(Graph *gd, int x, int *N1, int *N2, int *N3){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){ 
		if(N1[adj->v]==0 && N2[adj->v]==0) N3[adj->v]=1;
		adj=adj->next;
	}
}

int nullTab(int * tab, int taille){
	for(int i=0; i<taille; i++){
		if(tab[i]==1) return 0;
	}
	return 1;
}

int fullTab(int * tab, int taille){
	for(int i=0; i<taille; i++) if(tab[i]==0) return 0;
	return 1;
}

void rule1(Graph *g, int *cover){
	/*int memory[g->nbVertices];
	for(int i=0; i<g->nbVertices; i++){
		for(int j=0; j<g->nbVertices; j++) memory[i]=0;
		if(cover[i] && g->adjacencyLists!=NULL){
			adjacencyListElement *temp = g->adjacencyLists[i];
			while(temp!=NULL){
				if(cover[temp->v]){
					memory[temp->v]=1;
					printf("\n%d - %d\n", i, temp->v);
					afficherGraph(g);
					deleteNode(&g->adjacencyLists[temp->v], i);
					afficherGraph(g);
					getchar();
				}
				temp=temp->next;
			}
		}
		for(int j=0; j<g->nbVertices; j++) if(memory[j]) deleteNode(&g->adjacencyLists[i], j);
	}*/
}

void simplerules(Graph *g, int * cover){
	rule1(g, cover);
}

void reduceGraph(Graph *gd, int x) {
	for(int i=0; i<gd->nbVertices;i++){
		if(x==i){
			freeList(gd->adjacencyLists[i]);
			gd->adjacencyLists[i]=NULL;
		}
		else deleteNode(&gd->adjacencyLists[i], x);
	}
}

adjacencyListElement * undomlist(Graph *gd){
	adjacencyListElement *U = NULL;
	for(int i=0; i<gd->nbVertices; i++) if(gd->dom[i]==0 && gd->adjacencyLists[i]!=NULL) ajoute(&U, i);
	return U;
}

void branchedf(Graph *g, int *df){
	for(int i=0; i<g->nbVertices; i++) if(df[i] || g->adjacencyLists[i]==NULL) g->branched[i]=1;
}

char ** initMatC(int taille){
	char ** M = (char**) malloc(taille * sizeof(char*));
	for (int i = 0; i < taille; i++) {
  		M[i] = (char*) malloc(taille * sizeof(char));
	}
	return M;
}

void freeNs(int * N1,int * N2,int * N3){
	free(N1);
	free(N2);
	free(N3);
}