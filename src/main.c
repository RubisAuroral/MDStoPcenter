#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"

int main(){
	//Graph *g;
	Graph *gd;
	//char *file=argv[1];
	//g = initializeGraph(file);
	gd = cleanGraph(12);
	adjacencyListElement *tabN[gd->nbVertices][3];
	/*for(int i=0; i<g->nbVertices; i++){
		for(int j=0; j<g->nbVertices;j++){
			if(g->distanceMatrix[i][j]<140 && i!=j){
				Edge edge;
				edge.beginIdVertex=i+1;
				edge.endIdVertex=j+1;
				edge.weight=0;
				addEdge(gd,edge);
			}
		}
	}*/

	exemple(gd);
	adjacencyListElement *d0 = NULL;
	adjacencyListElement *df = NULL;
	adjacencyListElement *adel = NULL;
	while(count(gd, gd->nbVertices)==0){
		int x=bestToChoose(gd);
		ajoute(&d0, x);
		domine(x, gd);
	}

	for(int i=0; i<gd->nbVertices; i++) tabN[i][0]=createN1(gd, i);

	for(int i=0; i<gd->nbVertices; i++) tabN[i][1]=createN2(gd, i,tabN[i][0]);

	for(int i=0; i<gd->nbVertices; i++){
		tabN[i][2]=createN3(gd, i,tabN[i][0], tabN[i][1]);
		if(tabN[i][2]!=NULL){
			adjacencyListElement *temp = tabN[i][2];
			while(temp!=NULL){
				if(inL(adel,temp->v,-1)==0) ajoute(&adel, temp->v);
				temp=temp->next;
			}
			
			temp = tabN[i][1];
			while(temp!=NULL){
				if(inL(adel,temp->v,-1)==0) ajoute(&adel, temp->v);
				temp=temp->next;
			}
			ajoute(&df,i);
		} 
	}
	unDom(gd);
	domineliste(df, gd);
	adjacencyListElement *adeltemp = adel;
	while(adeltemp!=NULL){
	reduceGraph(gd,adeltemp->v);
		adeltemp=adeltemp->next;
	}
	free(adeltemp);
	afficheDom(gd);
	BnB(gd, df, undomlist(gd),d0);
}