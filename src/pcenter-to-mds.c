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
extern int info;
extern double d;

int compare ( const void * first, const void * second ) {
    int firstInt = * (const int *) first;
    int secondInt = * (const int *) second;
    return firstInt - secondInt;
}

/*Renvoie la plus grande distance de la matrice de distance de g*/
int distmax(Graph *g){
    int max=0;
    for(int i=0; i<g->nbVertices-1; i++){
        for(int j=i+1; j<g->nbVertices; j++){
            if(g->distanceMatrix[i][j]>max) max = g->distanceMatrix[i][j];
        }
    }
    return max;
}

/*Renvoie la plus petite distance de la matrice de distance de g*/
int distmin(Graph *g){
    int min=INT_MAX;
    for(int i=0; i<g->nbVertices-1; i++){
        for(int j=i+1; j<g->nbVertices; j++){
            if(g->distanceMatrix[i][j]<min) min = g->distanceMatrix[i][j];
        }
    }
    return min;
}

/*Renvoie le tableau de l'ensemble des distances unique de la matrice de distance de g*/
int *calcbornes(Graph *g){
    int taille = distmax(g)+1;
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
    for(int j=1; j<taille; j++){
        if(tabtemp[j]){
            tab[n]=j;
            n++;
        }
    }
    return tab;
}

/*Renvoie la n-eme plus petite distance de la matrice de distance de g*/
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

/*Effectue la Dichotomie de Chu Min LI -> D'abord on teste l'indice 1, puis l'indice 2, puis l'indice 4, puis 8... jusqu'à trouver une solution*/
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
        if(info) printf("Génération du graphe pour la distance %d", tab[actuel]);
        Graph *gtemp = cleanGraph(g->nbVertices);
        int nbedge = mdsgraph(gtemp, g, tab[actuel]);
        if(info) printf(" -> %d arcs\n", nbedge);
        created0(gtemp, d0);
        if(listeSize(d0, g->nbVertices) <= g -> p){ 
            if(info) printf("Glouton suffisant\n");
            freeGraph(gtemp);
            return actuel;
        } else{
            unDom(gtemp);
            int newnbedge=0; //nbedge-alber(gtemp, df);
            int solutionPartielle=listeSize(df, gtemp->nbVertices);
            if(info){
                if(solutionPartielle!=0) printf("Alber : Nombre de sommets fixés : %d | arcs restants : %d/%d | sommets restants : %d/%d\n", solutionPartielle, newnbedge,nbedge, listeSize(gtemp->ingraph, gtemp->nbVertices), gtemp->nbVertices);
                else printf("Alber : Pas de réduction\n");
            }

            if(solutionPartielle > g -> p){
                actuel*=2;
            } else{
                BnB(gtemp);
                if(listeSize(d0, g->nbVertices) <= g -> p){
                    if(info) printf("Solution trouvée\n");
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


/*Effectue l'inverse de la dichotomie de Chu Min (On commence par le dernier indice et on divise par 2)*/
int invdichotomieChuMin(Graph *g, int *tab){
    int actuel=tab[0]-1;
    while(1){
        end = clock();
        printf("\nBornes indices: ? - %d(d=%d) | %fs\n", actuel, tab[actuel], (double)(end - begin) / CLOCKS_PER_SEC);
        for(int i=0; i<g->nbVertices; i++){
            d0[i]=0;
            df[i]=0;
        }
        g->adom=g->nbVertices;
        if(info) printf("Génération du graphe pour la distance %d", tab[actuel]);
        Graph *gtemp = cleanGraph(g->nbVertices);
        int nbedge = mdsgraph(gtemp, g, tab[actuel]);
        if(info) printf(" -> %d arcs\n", nbedge);
        created0(gtemp, d0);
        if(listeSize(d0, g->nbVertices) <= g -> p){ 
            if(info) printf("Glouton suffisant\n");
            actuel/=2;
        } else{
            unDom(gtemp);
            int newnbedge=0; //nbedge-alber(gtemp, df);
            int solutionPartielle=listeSize(df, gtemp->nbVertices);
            if(info){
                if(solutionPartielle!=0) printf("Alber : Nombre de sommets fixés : %d | arcs restants : %d/%d | sommets restants : %d/%d\n", solutionPartielle, newnbedge,nbedge, listeSize(gtemp->ingraph, gtemp->nbVertices), gtemp->nbVertices);
                else printf("Alber : Pas de réduction\n");
            }
            if(solutionPartielle > g -> p){
                freeGraph(gtemp);
                return actuel;
            } else{
                BnB(gtemp);
                if(listeSize(d0, g->nbVertices) <= g -> p){
                    if(info) printf("Solution trouvée\n");
                    actuel/=2;
                }
                else{
                    freeGraph(gtemp);
                    return actuel;
                }
            }
        }
        freeGraph(gtemp);
    }
}

/*Effectue la dichotomie classique*/
int dichotomie(Graph *g, int *tab, int sup, int inf){
    while(sup!=inf){
		end = clock();
		printf("\nBornes indices: %d(d=%d) - %d(d=%d) | %fs\n", sup, tab[sup], inf, tab[inf], (double)(end - begin) / CLOCKS_PER_SEC);
		for(int i=0; i<g->nbVertices; i++){
			d0[i]=0;
			df[i]=0;
		}

		g->adom=g->nbVertices;
		int actuel=(sup+inf)*d;
		if(info) printf("Génération du graphe pour la distance %d (indice %d)", tab[actuel], actuel);
		Graph *gtemp = cleanGraph(g->nbVertices);
		int nbedge = mdsgraph(gtemp, g, tab[actuel]);
		if(info) printf(" -> %d arcs\n", nbedge);
		created0(gtemp, d0);

		if(listeSize(d0, g->nbVertices) <= g -> p){ 
			if(info) printf("Glouton suffisant\n");
			sup=actuel;
		} else{
			unDom(gtemp);
			int newnbedge=nbedge-alber(gtemp, df);
			int solutionPartielle=listeSize(df, gtemp->nbVertices);
			if(info){
                if(solutionPartielle!=0) printf("Alber : Nombre de sommets fixés : %d | arcs restants : %d/%d | sommets restants : %d/%d\n", solutionPartielle, newnbedge,nbedge, listeSize(gtemp->ingraph, gtemp->nbVertices), gtemp->nbVertices);
                else printf("Alber : Pas de réduction\n");
            }
			if(solutionPartielle > g -> p){
				if(info) printf("df est trop grand\n");
				inf=actuel+1;
			} else{
				BnB(gtemp);
				if(listeSize(d0, g->nbVertices) <= g -> p){
					if(info) printf("Solution trouvée\n");
					sup=actuel;
				}
				else{
					if(info) printf("Aucune solution trouvée\n");
					inf=actuel+1;
				}
			}
		}
		freeGraph(gtemp);
	}
    return inf;
}

/*Transforme le graphe origine en graphe dépondéré gtemp en fonction de la distance dist*/
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