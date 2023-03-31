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
	        if(r>=zb){zb=r; b=i;}
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

void domineliste(adjacencyListElement *df, Graph *g){
	adjacencyListElement *adom = df;
	while (adom!=NULL){
		domine(adom->v, g);
		adom=adom->next;
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

void unDom(Graph *g){
	for(int i=0; i<g->nbVertices;i++) g->dom[i]=0;
}

adjacencyListElement * createN1(Graph *gd, int x){
	adjacencyListElement *N1 = NULL;
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){
	        adjacencyListElement *adj2 = gd->adjacencyLists[adj->v];
	        while(adj2 != NULL && inL(gd->adjacencyLists[x], adj2->v, x)==1){
	        	adj2 = adj2->next;
	        }
	        if(adj2!=NULL) ajoute(&N1, adj->v);
	        adj = adj->next;
	}
	return N1;
}

adjacencyListElement * createN2(Graph *gd, int x, adjacencyListElement *L){
	adjacencyListElement *N2 = NULL;
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){
	        if(inL(L, adj->v, x)==0){
	        	adjacencyListElement *adj2 = gd->adjacencyLists[adj->v];
	        	int temp=inLv2(adj2, L);
	        	if(temp==1)ajoute(&N2, adj->v);
	        } 
	        adj = adj->next;
	}
	return N2;
}

adjacencyListElement * createN3(Graph *gd, int x, adjacencyListElement *N1, adjacencyListElement *N2){
	adjacencyListElement *N3 = NULL;
	adjacencyListElement *adj = gd->adjacencyLists[x];
	N3=difference(adj, N1);
	N3=difference(N3, N2);
	return N3;
}

void reduceGraph(Graph *gd, int x) {
	for(int i=0; i<gd->nbVertices;i++){
		if(x==i){
			gd->adjacencyLists[i]=NULL;
		}
		else gd->adjacencyLists[i]=deleteNode(gd->adjacencyLists[i], x);
	}
}

adjacencyListElement * undomlist(Graph *gd){
	adjacencyListElement *U = NULL;
	for(int i=0; i<gd->nbVertices; i++) if(gd->dom[i]==0) ajoute(&U, i);
	return U;
}
