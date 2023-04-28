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

void dominesave(int x, Graph *gd){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){
	    gd->save[adj->v]=1;
	    adj = adj->next;
	}
	gd->save[x]=1;
}

void domineliste(int *sol, Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(sol[i]==1){
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
	for(int i=0; i<g->nbVertices;i++) if(!g->save[i]) g->dom[i]=0;
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

void reduceGraph(Graph *gd, int x) {
	for(int i=0; i<gd->nbVertices;i++){
		if(x==i){
			freeList(gd->adjacencyLists[i]);
			gd->adjacencyLists[i]=NULL;
		}
		else deleteNode(&gd->adjacencyLists[i], x);
	}
}

void rule1(Graph *g){
	int memory[g->nbVertices];
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i]){
			for(int j=0; j<g->nbVertices; j++) memory[j]=0;
			adjacencyListElement *temp = g->adjacencyLists[i];
			while(temp!=NULL){
				memory[temp->v]=1;
				temp=temp->next;
			}
			for(int j=0; j<g->nbVertices; j++){
				if(memory[j] && g->dom[j] && j != i){
					memory[j]=1;
					deleteNode(&g->adjacencyLists[j], i);
					deleteNode(&g->adjacencyLists[i], j);
				}
			}
		}
	}
}

void rule2(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i] && degre(g, i)==1){
			reduceGraph(g,i);
		}
	}
}

void rule3v1(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		int firstV = 0;
		if(g->dom[i] && nbVoisin(g, i)==2){
			adjacencyListElement *temp =  g->adjacencyLists[i];
			firstV = temp->v;
			temp=temp->next;
			adjacencyListElement *voisin2 = g->adjacencyLists[temp->v];
			while(voisin2!=NULL){
				if(voisin2->v==firstV) reduceGraph(g,i);
				voisin2=voisin2->next;
			}
			temp=temp->next;
		} 
	}
}

void rule3v2(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i] && nbVoisin(g, i)==2){
			int firstV[g->nbVertices], secondV[g->nbVertices];
			for(int j=0; j<g->nbVertices; j++){
				firstV[j]=0; secondV[j]=0;
			}
			adjacencyListElement *temp =  g->adjacencyLists[i];
			adjacencyListElement *temp2 = g->adjacencyLists[temp->v];
			while(temp2!=NULL){
				firstV[temp2->v]=1;
				temp2=temp2->next;
			}
			temp=temp->next;
			adjacencyListElement *temp3 = g->adjacencyLists[temp->v];
			while(temp3!=NULL){
				secondV[temp3->v]=1;
				temp3=temp3->next;
			}
			for(int j=0; j<g->nbVertices; j++) if(firstV[j] && secondV[j]) reduceGraph(g,i);
		} 
	}
}


void rule4(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i] && nbVoisin(g, i)==3){
			int fv, sv, tv, test=0;
			adjacencyListElement *list = g->adjacencyLists[i];
			fv=list->v;
			list=list->next;
			sv=list->v;
			list=list->next;
			tv=list->v;
			adjacencyListElement *temp = g->adjacencyLists[fv];
			while(temp!=NULL){
				if(temp->v==sv) test=1;
				temp=temp->next;
			}
			if(test){
				test=0;
				adjacencyListElement *temp2 = g->adjacencyLists[sv];
				while(temp2!=NULL){
					if(temp2->v==tv) test=1;
					temp2=temp2->next;
				}
				if(test) reduceGraph(g, i);
			}
		}
	}
}

void simplerules(Graph *g){
	rule1(g);
	rule2(g);
	rule3v1(g);
	rule3v2(g);
	rule4(g);
}


adjacencyListElement * undomlist(Graph *gd){
	adjacencyListElement *U = NULL;
	for(int i=0; i<gd->nbVertices; i++) if(gd->dom[i]==0 && gd->adjacencyLists[i]!=NULL) ajoute(&U, i);
	return U;
}

void branchedf(Graph *g, int *df){
	for(int i=0; i<g->nbVertices; i++) if(df[i]) g->branched[i]=1;
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