#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include <sys/resource.h>
#include <time.h>

int k=0;
int *d0;
int *df;
Graph *g;
clock_t begin;
clock_t end;

int main(int argc, char *argv[]){
	begin = clock();
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
	/*FILE *fp;
    fp = fopen("test", "r");
	char line[256];
    while (fgets(line, sizeof(line), fp) != NULL){
        int num = atoi(line);
		domine(num, g);
	}
	afficheDom(g);
	getchar();*/
	
	for(int i=0; i<g->nbVertices;i++) g->dom[i]=0;
	int inN3;
	int rappel=-1;
	//for(int i=0; i<g->nbVertices; i++) createN3(g, i, N1, N2, N3);
	while(rappel<listeSize(df, g->nbVertices)){
		simplerules(g, g->dom);
		rappel=listeSize(df, g->nbVertices);
		printf("\nrappel : %d\n", rappel);
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
	afficherGraph(g);
	int yu=0, yi=0;
	for(int i=0; i<g->nbVertices; i++){
		if(!g->dom[i] && g->adjacencyLists[i]==NULL){
			df[i]=1;
			domine(i, g);
			dominesave(i, g);
		}
		yu+=df[i];
		yi+=d0[i];
	}
	printf("\ndf : %d - d0 : %d\n", yu, yi);
	printf("\n");
	afficherGraph(g);
	afficheDom(g);
	printf("\n");
	BnBtest();
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
	end = clock();
    printf("time for upgrade : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);
	//free(final);
	freeGraph(g);
	free(g);
}