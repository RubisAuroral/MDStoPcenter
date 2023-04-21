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
	
	char **N1 = initMatC(g->nbVertices);
	char **N2 = initMatC(g->nbVertices);
	char **N3 = initMatC(g->nbVertices);

	for(int j=0; j<g->nbVertices; j++){
		for(int i=0; i<g->nbVertices;i++){
			N1[j][i]=0;
			N2[j][i]=0;
			N3[j][i]=0;
		}
	}
	exemple(g, argv[2]);
	while(count(g, g->nbVertices)==0){
		int x=bestToChoose(g);
		d0[x]=1;
		domine(x, g);
	}
	afficheDom(g);
	for(int i=0; i<g->nbVertices; i++) createN1(g, i, N1);

	for(int i=0; i<g->nbVertices; i++) createN2(g, i, N1, N2);

	//for(int i=0; i<g->nbVertices; i++) createN3(g, i, N1, N2, N3);
	for(int i=0; i<g->nbVertices; i++){
		createN3(g, i, N1, N2, N3);
		int inN3 = 0;
		for(int j=0;j<g->nbVertices; j++){
			if(N3[i][j]==1){
				inN3=1;
				reduceGraph(g,j);
			}
		}
		if(inN3==1){
			inN3=0;
			for(int j=0;j<g->nbVertices; j++){
				if(N2[i][j]==1) reduceGraph(g,j);
			}
			df[i]=1;
		}
	}
	
	afficherGraph(g);
	branchedf(g,df);
	//adjacencyListElement *final=BnB(g, df, d0);
	int yu=0, yi=0;
	for(int i=0; i<g->nbVertices; i++){
		if(df[i]) yu++;
		if(d0[i]) yi++;
	}
	printf("df : %d - d0 : %d\n", yu, yi);
	unDom(g);
	domineliste(df, g);
	int * final = BnB3();
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
		if(final[i]){
			pitie++;
			printf("%d ", i);
		} 
	}
	printf(" (%d)", pitie);
	free(d0);
	free(df);
	freeNs(g->nbVertices,N1,N2,N3);
	freeGraph(g);
	free(g);
}