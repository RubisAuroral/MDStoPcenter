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
	if(argc>1) k=atoi(argv[1]);
	printf("Instance : %s\n", argv[2]);
	g = cleanGraph(atoi(argv[3]));
	d0=(int*)malloc(g->nbVertices*sizeof(int));
	df=(int*)malloc(g->nbVertices*sizeof(int));
	exemple(g, argv[2]);
	created0(g, d0);

	for(int i=0; i<g->nbVertices;i++) g->dom[i]=0; 
	alber(g, df);

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
	
	BnBtest();

	free(d0);
	free(df);
	end = clock();
    printf("time for end : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);

	freeGraph(g);
	free(g);
}