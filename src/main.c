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
int info=0;
double d=0;
Graph *g;
clock_t begin;
clock_t end;

int main(int argc, char *argv[]){
	int dicho=0;
	begin = clock();
	char *file=NULL;
	if(argc>9){
		fprintf(stderr,"Problème d'arguments : ./main (-k k) (-d coupeDichotomie) -f chemin_vers_instance\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=1; i<argc; i++){
		if(!strcmp(argv[i], "-f")){
			file = (char *) malloc(strlen(argv[i+1])*sizeof(char));
        	i++;
			strcpy(file, argv[i]);
		}
		if(!strcmp(argv[i], "-k")){
			i++;
			k=atoi(argv[i]);
		}
		if(!strcmp(argv[i], "-d")){
			i++;
			d=atoi(argv[i]);
		}
		if(!strcmp(argv[i], "-i")) info=1;
		if(!strcmp(argv[i], "-idc")) dicho=2;
		if(!strcmp(argv[i], "-dc")) dicho=1;
	}
	
	if (access(file, F_OK) == -1) {
        fprintf(stderr, "Le fichier %s n'existe pas ou vous n'avez pas les permissions pour y accéder.\n", file);
		exit(EXIT_FAILURE);
    }
	printf("Instance lue: %s\n", GET_FILENAME(file));
	if(!k) k=8;
	if(!d) d=50;
	d/=100;
	Graph *g = (Graph*)malloc(sizeof(*g));
	g = initializeGraph(file);
	p=g->p;
	
	if(info){
		printf("k nombre d'IS : %d\n", k);
		printf("Nombre de sommet du graphe d'origine : %d\n", g->nbVertices);
		printf("Taille de solution recherchée : %d\n", p);
	}

	d0 = (int *) malloc(g->nbVertices * sizeof(int));
	df = (int *) malloc(g->nbVertices * sizeof(int));

	int *bornes = NULL;
	bornes = calcbornes(g);
	Graph *gtemp = cleanGraph(g->nbVertices);
	mdsgraph(gtemp, g, 53);
	alber(gtemp, df);
	if(info){
		printf("Affichage de df : ");
		for(int i=0; i<gtemp->nbVertices; i++) if(df[i]) printf("%d ", i+1);
		printf("\n");
	}
	int max, min;
	if(dicho==0){
		min=1;
		max=bornes[0];
	}

	if(dicho==1){
		max = dichotomieChuMin(g, bornes);
		min = max / 2;
	}

	if(dicho==2){
		min = invdichotomieChuMin(g, bornes);
		max = min*2;
	}

	min = dichotomie(g, bornes, max, min);
	printf("\nIndice : %d - Opti : %d\n", min, bornes[min]);
	end = clock();
    printf("time for end : %fs\n", (double)(end - begin) / CLOCKS_PER_SEC);

	free(d0);
	free(df);
	freeGraph(g);
    for (int i = 0; i < g->nbVertices; i++) {
      free(g->distanceMatrix[i]);
    }
    free(g->distanceMatrix);
	free(g);
}