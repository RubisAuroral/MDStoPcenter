#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include "../headers/pcenter-to-mds.h"
#include <sys/resource.h>
#include <time.h>

extern clock_t begin;
extern clock_t end;
extern int *d0;
extern int *df;

int compare ( const void * first, const void * second ) {
    int firstInt = * (const int *) first;
    int secondInt = * (const int *) second;
    return firstInt - secondInt;
}

int distmax(Graph *g){
    int max=0;
    for(int i=0; i<g->nbVertices-1; i++){
        for(int j=i+1; j<g->nbVertices; j++){
            if(g->distanceMatrix[i][j]>max) max = g->distanceMatrix[i][j];
        }
    }
    return max;
}

int distmin(Graph *g){
    int min=INT_MAX;
    for(int i=0; i<g->nbVertices-1; i++){
        for(int j=i+1; j<g->nbVertices; j++){
            if(g->distanceMatrix[i][j]<min) min = g->distanceMatrix[i][j];
        }
    }
    return min;
}

int *calcbornes(Graph *g){
    int taille = distmax(g);
    int tabtemp[taille];
    memset(tabtemp, 0, taille * sizeof(int));
    for(int i=0; i<g->nbVertices-1; i++){
        for(int j=i+1; j<g->nbVertices; j++){
            tabtemp[g->distanceMatrix[i][j]]=1;
        }
    }
    int t2 = listeSize(tabtemp, taille)+1;
    int *tab = malloc(t2 * sizeof(int));
    tab[0]=t2;
    int n=1;
    printf("sommets skip : ");
    for(int j=1; j<taille; j++){
        if(tabtemp[j]){
            tab[n]=j;
            n++;
        }
        else{
            printf("%d ", j);
        }
    }
    printf("\n");
    return tab;
}

int distmin2(Graph *g, int p){
    int valeurs[g->nbVertices * (g->nbVertices - 1) / 2]; // Tableau pour stocker les valeurs de distance
    int index = 0;
    for(int i=0; i<g->nbVertices-1; i++){
        for(int j=i+1; j<g->nbVertices; j++){
            valeurs[index] = g->distanceMatrix[i][j];
            index++;
        }
    }
    qsort(valeurs, g->nbVertices * (g->nbVertices - 1) / 2, sizeof(int), compare);
    return valeurs[p];
}

int dichotomieChuMin(Graph *g, int *tab){
    int actuel=1;
    while(1){
        end = clock();
        printf("\nBornes indices: %d(d=%d) - ? | %fs\n", actuel, tab[actuel], (double)(end - begin) / CLOCKS_PER_SEC);
        for(int i=0; i<g->nbVertices; i++){
            d0[i]=0;
            df[i]=0;
        }
        g->adom=g->nbVertices;
        printf("Génération du graphe pour la distance %d", tab[actuel]);
        Graph *gtemp = cleanGraph(g->nbVertices);
        int nbedge = mdsgraph(gtemp, g, tab[actuel]);
        printf(" -> %d arcs\n", nbedge);
        created0(gtemp, d0);
        if(listeSize(d0, g->nbVertices) <= g -> p){ 
            printf("Glouton suffisant\n");
            freeGraph(gtemp);
            return actuel;
        } else{
            unDom(gtemp);
            int newnbedge=0; //nbedge-alber(gtemp, df);
            int solutionPartielle=listeSize(df, gtemp->nbVertices);
            if(solutionPartielle!=0) printf("Alber : Nombre de sommets fixés : %d | arcs restants : %d/%d | sommets restants : %d/%d\n", solutionPartielle, newnbedge,nbedge, listeSize(gtemp->ingraph, gtemp->nbVertices), gtemp->nbVertices);
            else printf("Alber : Pas de réduction\n");

            if(solutionPartielle > g -> p){
                actuel*=2;
            } else{
                BnB(gtemp);
                if(listeSize(d0, g->nbVertices) <= g -> p){
                    printf("Solution trouvée\n");
                    freeGraph(gtemp);
                    return actuel;
                }
                else{
                    actuel*=2;
                }
            }
        }
        freeGraph(gtemp);
    }
}

int dichotomie(Graph *g, int *tab, int sup, int inf){
    while(sup!=inf){
		end = clock();
		printf("\nBornes indices: %d(d=%d) - %d(d=%d) | %fs\n", sup, tab[sup], inf, tab[inf], (double)(end - begin) / CLOCKS_PER_SEC);
		for(int i=0; i<g->nbVertices; i++){
			d0[i]=0;
			df[i]=0;
		}

		g->adom=g->nbVertices;
		int actuel=(sup+inf)/2;
		printf("Génération du graphe pour la distance %d (indice %d)", tab[actuel], actuel);
		Graph *gtemp = cleanGraph(g->nbVertices);
		int nbedge = mdsgraph(gtemp, g, tab[actuel]);
		printf(" -> %d arcs\n", nbedge);
		created0(gtemp, d0);

		if(listeSize(d0, g->nbVertices) <= g -> p){ 
			printf("Glouton suffisant\n");
			sup=actuel;
		} else{
			unDom(gtemp);
			int newnbedge=nbedge-alber(gtemp, df);
			int solutionPartielle=listeSize(df, gtemp->nbVertices);
			if(solutionPartielle!=0) printf("Alber : Nombre de sommets fixés : %d | arcs restants : %d/%d | sommets restants : %d/%d\n", solutionPartielle, newnbedge,nbedge, listeSize(gtemp->ingraph, gtemp->nbVertices), gtemp->nbVertices);
			else printf("Alber : Pas de réduction\n");

			if(solutionPartielle > g -> p){
				printf("df est trop grand\n");
				inf=actuel+1;
			} else{
				BnB(gtemp);
				if(listeSize(d0, g->nbVertices) <= g -> p){
					printf("Solution trouvée\n");
					sup=actuel;
				}
				else{
					printf("Aucune solution trouvée\n");
					inf=actuel+1;
				}
			}
		}
		freeGraph(gtemp);
	}
    return inf;
}


int mdsgraph(Graph *gtemp, Graph *origine, int dist){
    int nbedge=0;
    Edge edge;
    edge.weight=0;
    gtemp->p=origine->p;
    for(int i=0; i<gtemp->nbVertices-1; i++){
        for(int j=i+1; j<gtemp->nbVertices; j++){
            if(origine->distanceMatrix[i][j]<=dist){
                edge.beginIdVertex=i+1;
                edge.endIdVertex=j+1;
                addEdge(gtemp, edge);
                addInverseEdge(gtemp, edge);
                nbedge++;
            }
        }
    }
    return nbedge;
}