#include "../headers/p_center.h"
#include "../headers/misc.h"

int bestToChoose(Graph *gd){
	int r,zb=0, b=0;
	for (int i = 0; i < gd->nbVertices; i++){
		r=nbVoisinv2(gd, i);
	    if(r>zb){
			zb=r; 
			b=i;
		}
	}
	return b;
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

int alldom(Graph *gd, int x){
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
	        if(!N1[i]){
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
	int x=listeSize(tab, taille);
	if(x==taille) return 1;
	else return 0;
}

void reduceGraph(Graph *gd, int x) {
	gd->ingraph[x]=0;
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
				if(memory[j] && g->dom[j]){
					//printf("suppr1 : %d-%d\n", i,j);
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
			//printf("suppr2 : %d\n", i);
		}
	}
}

void rule3v1(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		int firstV;
		if(g->dom[i] && nbVoisin(g, i)==2 && degre(g,i)==2){
			adjacencyListElement *temp =  g->adjacencyLists[i];
			firstV = temp->v;
			temp=temp->next;
			adjacencyListElement *voisin2 = g->adjacencyLists[temp->v];
			while(voisin2!=NULL){
				if(voisin2->v==firstV){
					reduceGraph(g,i);
					//printf("suppr3 : %d\n", i);
				}
				voisin2=voisin2->next;
			}
			temp=temp->next;
		} 
	}
}

void rule3v2(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i] && nbVoisin(g, i)==2 && degre(g,i)==2){
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
			for(int j=0; j<g->nbVertices; j++) if(firstV[j] && secondV[j] && i!=j){
				//printf("suppr3 : %d\n", i);
				reduceGraph(g,i);
				break;
			}
		} 
	}
}


void rule4(Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i] && nbVoisin(g, i)==3 && degre(g,i)==3){
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

void branchedf(Graph *g, int *df){
	for(int i=0; i<g->nbVertices; i++) if(df[i]) g->branched[i]=1;
}

void created0(Graph *g, int * d0){
	while(!fullTab(g->dom, g->nbVertices)){
		int x=bestToChoose(g);
		d0[x]=1;
		domine(x, g);
	}
}

void printpreproc(Graph *g, int *df, int *d0){
	int tailledf=0, tailled0=0;
	for(int i=0; i<g->nbVertices; i++){
		if(!g->dom[i] && g->adjacencyLists[i]==NULL){
			df[i]=1;
			domine(i, g);
			dominesave(i, g);
			g->ingraph[i]=1;
		}
		tailledf+=df[i];
		tailled0+=d0[i];
	}
	int remaining=0;
	for(int i=0; i<g->nbVertices; i++) if(g->ingraph[i]) remaining++;
	
	printf("Il reste %d sommets potentiels\n", remaining);
	printf("Nombre de sommets fixés: %d - Best actuel : %d\n", tailledf, tailled0);
}

void alber(Graph *g, int * df){
	int inN3;
	int N1[g->nbVertices];
	int N2[g->nbVertices];
	int N3[g->nbVertices];
	int rappel=-1;
	while(rappel<listeSize(df, g->nbVertices)){
		simplerules(g);
		rappel=listeSize(df, g->nbVertices);
		//printf("\nrappel : %d\n", rappel);
		for(int i=0; i<g->nbVertices; i++){
			inN3=0;
			for(int j=0; j<g->nbVertices;j++){
				N1[j]=0;
				N2[j]=0;
				N3[j]=0;
			}
			createN1(g, i, N1);
			createN2(g, i, N1, N2);
			createN3(g, i, N1, N2, N3);
			for(int j=0;j<g->nbVertices; j++) if(N3[j]==1 && !g->dom[j]) inN3=1;
			if(inN3==1){
				domine(i, g);
				dominesave(i,g);
				reduceGraph(g, i);
				for(int j=0;j<g->nbVertices; j++){
					if(N2[j] || N3[j]){
						reduceGraph(g,j);
					} 
				}
				df[i]=1;
			}
		}    
	}
}