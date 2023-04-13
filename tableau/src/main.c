#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"

int k=0;
int *d0;
int *df;

int main(int argc, char *argv[]){
	Graph *gd;
	if(argc>1)k=atoi(argv[1]);
	gd = cleanGraph(658);
	d0=(int*)malloc(gd->nbVertices*sizeof(int));
	df=(int*)malloc(gd->nbVertices*sizeof(int));
	
	char **N1 = initMatC(gd->nbVertices);
	char **N2 = initMatC(gd->nbVertices);
	char **N3 = initMatC(gd->nbVertices);

	for(int j=0; j<gd->nbVertices; j++){
		for(int i=0; i<gd->nbVertices;i++){
			N1[j][i]=0;
			N2[j][i]=0;
			N3[j][i]=0;
		}
	}
	exemple(gd);
	while(count(gd, gd->nbVertices)==0){
		int x=bestToChoose(gd);
		d0[x]=1;
		domine(x, gd);
	}
	afficheDom(gd);

	for(int i=0; i<gd->nbVertices; i++) createN1(gd, i, N1);

	for(int i=0; i<gd->nbVertices; i++) createN2(gd, i, N1, N2);

	for(int i=0; i<gd->nbVertices; i++) createN3(gd, i, N1, N2, N3);
	
	for(int i=0; i<gd->nbVertices; i++){
		int inN3 = 0;
		for(int j=0;j<gd->nbVertices; j++){
			if(N3[i][j]==1){
				inN3=1;
				reduceGraph(gd,j);
			}
		}
		if(inN3==1){
			inN3=0;
			for(int j=0;j<gd->nbVertices; j++){
				if(N2[i][j]==1) reduceGraph(gd,j);
			}
			df[i]=1;
		}
	}

	afficherGraph(gd);
	branchedf(gd,df);
	//adjacencyListElement *final=BnB(gd, df, d0);
	adjacencyListElement *final=BnB2(gd);
	/*printf("\nFINAL (%d): ", listeSize(final));
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
	afficheListe(final);*/
	free(d0);
	free(df);
	freeNs(gd->nbVertices,N1,N2,N3);
	freeGraph(gd);
	free(gd);
}