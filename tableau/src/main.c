#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"

int k=0;
int *d0 = NULL;
int *df = NULL;

int main(int argc, char *argv[]){
	Graph *gd;
	if(argc>1)k=atoi(argv[1]);
	gd = cleanGraph(658);
	int *tabN[gd->nbVertices][3];
	exemple(gd);
	
	while(count(gd, gd->nbVertices)==0){
		int x=bestToChoose(gd);
		ajoute(&d0, x);
		domine(x, gd);
	}
	/*
	for(int i=0; i<gd->nbVertices; i++) tabN[i][0]=createN1(gd, i);
	for(int i=0; i<gd->nbVertices; i++) tabN[i][1]=createN2(gd, i,tabN[i][0]);
	for(int i=0; i<gd->nbVertices; i++){
		tabN[i][2]=createN3(gd, i,tabN[i][0], tabN[i][1]);
		if(tabN[i][2]!=NULL){
			adjacencyListElement *temp = tabN[i][2];
			while(temp!=NULL){
				reduceGraph(gd,temp->v);
				temp=temp->next;
			}
			temp = tabN[i][1];
			while(temp!=NULL){
				reduceGraph(gd,temp->v);
				temp=temp->next;
			}
			ajoute(&df,i);
		}
	}
	afficherGraph(gd);
	getchar();
	branchedf(gd,df);
	//adjacencyListElement *final=BnB(gd, df, d0);
	adjacencyListElement *final=BnB2(gd);
	printf("\nFINAL (%d): ", listeSize(final));
	/*for(int i=0; i<16; i++){
		for(int j=i+1; j<16;j++){
			adjacencyListElement *testopt = NULL;
			ajoute(&testopt, i);
			ajoute(&testopt, j);
			domineliste(testopt, gd);
			printf("%d - %d : ", i+1,j+1);
			afficheDom(gd);
			unDom(gd);
		}
	}
	afficheListe(final);
	freeGraph(gd);*/
}