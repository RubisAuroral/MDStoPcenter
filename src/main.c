#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include <sys/resource.h>

int k=0;
int *d0;
int *df;
Graph *g;

int main(int argc, char *argv[]){
	if(argc>1)k=atoi(argv[1]);
	g = cleanGraph(atoi(argv[3]));
	d0=(int*)malloc(g->nbVertices*sizeof(int));
	df=(int*)malloc(g->nbVertices*sizeof(int));
	
	int N1[g->nbVertices];
	int N2[g->nbVertices];
	int N3[g->nbVertices];
	
	exemple(g, argv[2]);
	while(count(g, g->nbVertices)==0){
		int x=bestToChoose(g);
		d0[x]=1;
		domine(x, g);
	}
	afficheDom(g);
	int covered[g->nbVertices];
	for(int j=0; j<g->nbVertices;j++){
		covered[j]=0;
	}
	int inN3;
	int rappel=-1;
	//for(int i=0; i<g->nbVertices; i++) createN3(g, i, N1, N2, N3);
	while(rappel<listeSize(df, g->nbVertices)){
		rappel=listeSize(df, g->nbVertices);
		printf("rappel : %d\n", rappel);
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
			for(int j=0;j<g->nbVertices; j++) if(N3[j]==1 && !covered[j]) inN3=1;
			if(inN3==1){
				adjacencyListElement *temp=g->adjacencyLists[i];
				while(temp!=NULL){
					covered[temp->v]=1;
					temp=temp->next;
				}
				reduceGraph(g, i);
				for(int j=0;j<g->nbVertices; j++){
					if(N2[j] || N3[j]){
						reduceGraph(g,j);
					} 
				}
				df[i]=1;
			}
		}
		simplerules(g, covered);
	}
	
	afficherGraph(g);
	branchedf(g,df);
	//adjacencyListElement *final=BnB(g, df, d0);
	int yu=0, yi=0;
	printf("df : ");
	for(int i=0; i<g->nbVertices; i++){
		if(df[i]){
			yu++;
			printf("%d ", i);
		}
		if(d0[i]) yi++;
	}
	printf("df : %d - d0 : %d\n", yu, yi);
	unDom(g);
	domineliste(df, g);
	BnBtest();
	//printf("\nFINAL (%d): ", listeSize(final));
	/*for(int i=0; i<16; i++){
		for(int j=i+1; j<16;j++){
			adjacencyListElement *testopt = NULL;
			ajoute(&testopt, i);
			ajoute(&testopt, j);
			domineliste(testopt, g);
			printf("%d - %d : ", i+1,j+1);
			afficheDom(g);
			unDom(g);
		}
	}
	afficheListe(final);*/
	//printf("oui ?\n");
	int pitie=0;
	for(int i=0; i<g->nbVertices; i++){
		if(d0[i]){
			pitie++;
			printf("%d ", i);
		} 
	}
	printf(" (%d)", pitie);
	free(d0);
	free(df);
	freeGraph(g);
	free(g);
}
