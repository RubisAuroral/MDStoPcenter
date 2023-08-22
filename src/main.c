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
	double d=0;
	if(argc>7){
		fprintf(stderr,"Problème d'arguments : ./main (-k k) (-d coupeDichotomie) -f chemin_vers_instance\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=1; i<argc-1; i+=2){
		if(!strcmp(argv[i], "-f")){
			file = (char *) malloc(strlen(argv[i+1])*sizeof(char));
        	strcpy(file, argv[i+1]);
		}
		if(!strcmp(argv[i], "-k")) k=atoi(argv[i+1]);
		if(!strcmp(argv[i], "-d")) d=atoi(argv[i+1]);
	}
	
	if (access(file, F_OK) == -1) {
        fprintf(stderr, "Le fichier %s n'existe pas ou vous n'avez pas les permissions pour y accéder.\n", file);
		exit(EXIT_FAILURE);
    }

	printf("Instance lue: %s\n", GET_FILENAME(file));
	if(!k) k=8;
	if(!d) d=50;
	d/=100;
	printf("k nombre d'IS : %d\n", k);

	Graph *g = (Graph*)malloc(sizeof(*g));
	g = initializeGraph(file);
	printf("Nombre de sommet du graphe d'origine : %d\n", g->nbVertices);
	p=g->p;
	printf("Taille de solution recherchée : %d\n", p);

	d0 = (int *) malloc(g->nbVertices * sizeof(int));
	df = (int *) malloc(g->nbVertices * sizeof(int));

	int *bornes = NULL;
	bornes = calcbornes(g);
	int max = bornes[0]-1;
	int min = 1;
	min = dichotomie(g, bornes, max, min);
	printf("\nIndice : %d - Opti : %d\n", min, bornes[min]);
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