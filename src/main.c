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
	unDom(g);
	alber(g, df);
	printpreproc(g, df, d0);
	BnB(g);

	free(d0);
	free(df);
	end = clock();
    printf("time for end : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);

	freeGraph(g);
	free(g);
}