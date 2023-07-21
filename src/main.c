#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include "../headers/pcenter-to-mds.h"
#include <sys/resource.h>
#include <time.h>

#define GET_FILENAME(path) (strrchr(path, '/') ? strrchr(path, '/') + 1 : path)

int k=0;
int all=0;
int cuted=0;
int *d0;
int *df;
int p;
Graph *g;
clock_t begin;
clock_t end;

int main(int argc, char *argv[]){
	begin = clock();
	char *file=NULL;
	if(argc>5){
		fprintf(stderr,"Problème d'arguments : ./main (-k k) -f chemin_vers_instance\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=1; i<argc-1; i+=2){
		if(!strcmp(argv[i], "-f")){
			file = (char *) malloc(strlen(argv[i+1])*sizeof(char));
        	strcpy(file, argv[i+1]);
		}
		if(!strcmp(argv[i], "-k")) k=atoi(argv[i+1]);
	}
	
	if (access(file, F_OK) == -1) {
        fprintf(stderr, "Le fichier %s n'existe pas ou vous n'avez pas les permissions pour y accéder.\n", file);
		exit(EXIT_FAILURE);
    }

	printf("Instance lue: %s\n", GET_FILENAME(file));
	if(!k) k=8;
	printf("k nombre d'IS : %d\n", k);

	Graph *g = (Graph*)malloc(sizeof(*g));
	g = initializeGraph(file);
	printf("Nombre de sommet du graphe d'origine : %d\n", g->nbVertices);
	p=g->p;
	printf("Taille de solution recherchée : %d\n", p);

	d0 = (int *) malloc(g->nbVertices * sizeof(int));
	df = (int *) malloc(g->nbVertices * sizeof(int));

	int max = distmax(g);
	int min = distmin(g);

	while(max!=min){
		end = clock();
		printf("\nBornes : %d - %d | %fs\n", max, min, (double)(end - begin) / CLOCKS_PER_SEC);
		for(int i=0; i<g->nbVertices; i++){
			d0[i]=0;
			df[i]=0;
		}

		g->adom=g->nbVertices;
		int actuel=(max+min)/2;
		printf("Génération du graphe pour la distance %d", actuel);
		Graph *gtemp = cleanGraph(g->nbVertices);
		int nbedge = mdsgraph(gtemp, g, actuel);
		printf(" -> %d arcs\n", nbedge);
		created0(gtemp, d0);

		if(listeSize(d0, g->nbVertices) <= g -> p){ 
			printf("Glouton suffisant\n");
			max=actuel;
		} else{
			unDom(gtemp);
			int newnbedge=nbedge-alber(gtemp, df);
			int solutionPartielle=listeSize(df, gtemp->nbVertices);
			if(solutionPartielle!=0) printf("Alber : Nombre de sommets fixés : %d | arcs restants : %d/%d | sommets restants : %d/%d\n", solutionPartielle, newnbedge,nbedge, listeSize(gtemp->ingraph, gtemp->nbVertices), gtemp->nbVertices);
			else printf("Alber : Pas de réduction\n");

			if(solutionPartielle > g -> p){
				printf("df est trop grand\n");
				min=actuel+1;
			} else{
				BnB(gtemp);
				if(listeSize(d0, g->nbVertices) <= g -> p){
					printf("Solution trouvée\n");
					max=actuel;
				}
				else{
					printf("Aucune solution trouvée\n");
					min=actuel+1;
				}
			}
		}
		freeGraph(gtemp);
	}

	printf("\nOpti : %d\n", min);
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