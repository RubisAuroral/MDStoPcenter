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
	int *final;
	exemple(g, argv[2]);
	while(count(g, g->nbVertices)==0){
		int x=bestToChoose(g);
		d0[x]=1;
		domine(x, g);
	}
	for(int i=0;i<g->nbVertices; i++) printf("%d", d0[i]);
	/*FILE *fp;
    fp = fopen("test", "r");
	char line[256];
    while (fgets(line, sizeof(line), fp) != NULL){
        int num = atoi(line);
		domine(num, g);
	}
	afficheDom(g);
	getchar();*/
	
	unDom(g);
	int rappel=-1;
	//for(int i=0; i<g->nbVertices; i++) createN3(g, i, N1, N2, N3);
	while(rappel<listeSize(df, g->nbVertices)){
		rappel=listeSize(df, g->nbVertices);
		printf("\nrappel : %d\n", rappel);
		simplerules(g, g);
		Albertrule1(g, g, df, 1);
	}
	branchedf(g,df);
	int yu=0, yi=0;
	printf("df : ");
	for(int i=0; i<g->nbVertices; i++){
		if(!g->dom[i] && g->adjacencyLists[i]==NULL){
			df[i]=1;
			domine(i, g);
			dominesave(i, g);
		}
		if(df[i]){
			yu++;
			printf("%d ", i);
		}
		if(d0[i]) yi++;
	}
	printf("\ndf : %d - d0 : %d\n", yu, yi);
	for(int i=0; i<g->nbVertices; i++) if(!g->branched[i]) printf("%d ", i);
	printf("\n");
	afficherGraph(g);
	getchar();
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
    printf("time for end : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);
	//free(final);
	freeGraph(g);
	free(g);
}