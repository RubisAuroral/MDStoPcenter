#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include "../headers/pcenter-to-mds.h"
#include "../headers/main-emos-mds.h"
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
	if(argc!=3) return -1;
	k=atoi(argv[1]);
	//printf("Instance : %s\n", argv[2]);
	Graph *g = (Graph*)malloc(sizeof(*g));
	g = initializeGraph(argv[2]);
	//afficherGraph(g);
	//g = cleanGraph(atoi(argv[3]));
	d0=(int*)malloc(g->nbVertices*sizeof(int));
	df=(int*)malloc(g->nbVertices*sizeof(int));
	//exemple(g, argv[2]);
	int min = distmax(g), max = distmin(g);
	int scoring[min];
	for(int i=0; i<min; i++) scoring[i]=-1;
	
	
	while(min!=max){
		if(scoring[min]!=-1 && scoring[min]==scoring[max]) break;
		end = clock();
		printf("%d - %d - %fs\n", min, max, (double)(end - begin) / CLOCKS_PER_SEC);
		for(int i=0; i<g->nbVertices; i++){
			d0[i]=0;
			df[i]=0;
			g->save[i]=0;
		}
		int actuel=(min+max)/2;
		Graph *gtemp = cleanGraph(g->nbVertices);
		mdsgraph(gtemp, g, actuel);
		created0(gtemp, d0);
		if(listeSize(d0, g->nbVertices) <= g -> p){ 
			min=actuel;
			scoring[min]=listeSize(d0, g->nbVertices);
		}
		else{
			unDom(gtemp);
			alber(gtemp, df);
			if(listeSize(df, g->nbVertices) > g -> p){
				max=actuel+1;
				scoring[max]=listeSize(df, g->nbVertices);
			}
			else{
				instanceHua(gtemp);
				int temp = final(argc, "insttemp")
				if(listeSize(d0, g->nbVertices) <= g -> p){
					min=actuel;
					scoring[min]=listeSize(d0, g->nbVertices);
				}
				else{
					max=actuel+1;
					scoring[max]=listeSize(d0, g->nbVertices);
				}
			}
		}
	}

	/*Graph *gtemp = cleanGraph(g->nbVertices);
	mdsgraph(gtemp, g, 56);
	created0(gtemp, d0);
	unDom(gtemp);
	alber(gtemp, df);
	BnB(gtemp);*/
	//afficherGraph(gtemp);
	//instanceHua(gtemp);

	printf("Opti : %d\n", max);
	/*created0(gtemp, d0);
	unDom(gtemp);
	alber(gtemp, df);
	printpreproc(gtemp, df, d0);
	BnB(gtemp);*/

	free(d0);
	free(df);
	end = clock();
    printf("time for end : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);

	freeGraph(g);
    for (int i = 0; i < g->nbVertices; i++) {
      free(g->distanceMatrix[i]);
    }
    free(g->distanceMatrix);
	free(g);
}