#include "../headers/p_center.h"
#include "../headers/misc.h"


/*Choisit le meilleur sommet du grpahe -> Celui qui en dominerait le plus dans l'état actuel*/
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

/*Domine le sommet x et tout ses voisins*/
void domine(int x, Graph *gd){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){
	    if(!gd->dom[adj->v]) gd->adom--;
		gd->dom[adj->v]=1;
	    adj = adj->next;
	}
	if(!gd->dom[x]) gd->adom--;
	gd->dom[x]=1;
}

/*Domine définitivement le sommet x et ses voisins -> Utilisé uniquement quand des sommets sont ajoutés par Alber*/
void dominesave(int x, Graph *gd){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){
	    gd->save[adj->v]=1;
	    adj = adj->next;
	}
	gd->save[x]=1;
}

/*Domine une liste de sommets*/
void domineliste(int *sol, Graph *g){
	for(int i=0; i<g->nbVertices; i++){
		if(sol[i]==1){
			domine(i, g);
		}
	}
}

/*Vérifie si tous les sommets sont dominés*/
int alldom(Graph *gd, int x){
	for(int i=0; i<x; i++){
		if(gd->dom[i]==0) return 0;
	}
	return 1;
}

/*Affiche tous les sommets dominés*/
void afficheDom(Graph *gd){
	for(int i=0; i<gd->nbVertices; i++) printf("%d", gd->dom[i]);
	printf("\n");
}

/*Affiche tous les sommets branchés*/
void afficheBranched(Graph *gd){
	for(int i=0; i<gd->nbVertices; i++) printf("%d", gd->branched[i]);
	printf("\n");
}

/*Rend non dominé tous les sommets du graphe (sauf ceux qui sont dominés définitivement)*/
void unDom(Graph *g){
	for(int i=0; i<g->nbVertices;i++) if(!g->save[i] && g->dom[i]){
		g->dom[i]=0;
		g->adom++;
	}
}

/*Créé l'ensemble N1 pour Alber*/
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

/*Créé l'ensemble N2 pour Alber*/
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

/*Créé l'ensemble N3 pour Alber*/
void createN3(Graph *gd, int x, int *N1, int *N2, int *N3){
	adjacencyListElement *adj = gd->adjacencyLists[x];
	while (adj != NULL){ 
		if(N1[adj->v]==0 && N2[adj->v]==0) N3[adj->v]=1;
		adj=adj->next;
	}
}

/*Vérifie si un tableau est vide*/
int nullTab(int * tab, int taille){
	for(int i=0; i<taille; i++){
		if(tab[i]==1) return 0;
	}
	return 1;
}

/*Vérifie si un tableau est plein*/
int fullTab(int * tab, int taille){
	int x=listeSize(tab, taille);
	if(x==taille) return 1;
	else return 0;
}

/*Supprime le sommet x du graphe gd*/
int reduceGraph(Graph *gd, int x) {
	int tmp=nbVoisin(gd,x);
	gd->ingraph[x]=0;
	for(int i=0; i<gd->nbVertices;i++){
		if(x==i){
			freeList(gd->adjacencyLists[i]);
			gd->adjacencyLists[i]=NULL;
		}
		else deleteNode(&gd->adjacencyLists[i], x);
	}
	return tmp;
}

/*Applique la simplerules 1*/
int rule1(Graph *g){
	int tmp=0;
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
					tmp++;
				}
			}
		}
	}
	return tmp;
}

/*Applique la simplerules 2*/
int rule2(Graph *g){
	int tmp=0;
	for(int i=0; i<g->nbVertices; i++){
		if(g->dom[i] && degre(g, i)==1){
			tmp+=reduceGraph(g,i);
			//printf("suppr2 : %d\n", i);
		}
	}
	return tmp;
}

/*Applique la simplerules 3*/
int rule3v1(Graph *g){
	int tmp=0;
	for(int i=0; i<g->nbVertices; i++){
		int firstV;
		if(g->dom[i] && nbVoisin(g, i)==2 && degre(g,i)==2){
			adjacencyListElement *temp =  g->adjacencyLists[i];
			firstV = temp->v;
			temp=temp->next;
			adjacencyListElement *voisin2 = g->adjacencyLists[temp->v];
			while(voisin2!=NULL){
				if(voisin2->v==firstV){
					tmp+=reduceGraph(g,i);
					//printf("suppr3 : %d\n", i);
				}
				voisin2=voisin2->next;
			}
			temp=temp->next;
		} 
	}
	return tmp;
}

/*Applique la variante de la simplerules 3*/
int rule3v2(Graph *g){
	int tmp=0;
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
				tmp+=reduceGraph(g,i);
				break;
			}
		} 
	}
	return tmp;
}

/*Applique la simplerules 4*/
int rule4(Graph *g){
	int tmp=0;
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
				if(test) tmp+=reduceGraph(g,i);
			}
		}
	}
	return tmp;
}

/*Applique toutes les simplesrules*/
int simplerules(Graph *g){
	int delBySimpleRules=0;
	delBySimpleRules+=rule1(g);
	delBySimpleRules+=rule2(g);
	delBySimpleRules+=rule3v1(g);
	delBySimpleRules+=rule3v2(g);
	delBySimpleRules+=rule4(g);
	return delBySimpleRules;
}

/*Branche tous les sommets qui sont dans df*/
void branchedf(Graph *g, int *df){
	for(int i=0; i<g->nbVertices; i++) if(df[i]) g->branched[i]=1;
}

/*Créé la solution glouton*/
void created0(Graph *g, int * d0){
	while(g->adom!=0){
		int x=bestToChoose(g);
		d0[x]=1;
		domine(x, g);
	}
}

/*Affiche les résultats du preprocessing*/
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
/*Applique Alber*/
int alber(Graph *g, int * df){
	int tmp=0;
	int inN3;
	int N1[g->nbVertices];
	int N2[g->nbVertices];
	int N3[g->nbVertices];
	int rappel=-1;
	while(rappel<listeSize(df, g->nbVertices)){
		tmp+=simplerules(g);
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
				tmp+=reduceGraph(g,i);
				for(int j=0;j<g->nbVertices; j++){
					if(N2[j] || N3[j]){
						tmp+=reduceGraph(g,j);
					} 
				}
				df[i]=1;
			}
		}    
	}
	return tmp;
}