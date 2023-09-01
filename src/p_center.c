/*
FICHIER DE THOMAS BAZAILLE
*/


#include "../headers/p_center.h"

void addEdge(Graph *g, Edge edge){

    bool doublonFound = false;
    adjacencyListElement *doublon = g->adjacencyLists[edge.beginIdVertex - 1]; 
    while (doublon != NULL){
        if (doublon->v == edge.endIdVertex - 1 && doublon->weight != edge.weight){
            doublon->weight = edge.weight;
            doublonFound = true;
            break;
        }
        doublon = doublon->next;
    }
    if (!doublonFound){
        adjacencyListElement* adj = (adjacencyListElement *)malloc(sizeof(adjacencyListElement));
        if (adj == NULL){exit(0);}
        adj->v = edge.endIdVertex - 1; //on enlève 1 pour matcher avec le format des fichiers pmed
        adj->weight = edge.weight;
        adj->next = g->adjacencyLists[edge.beginIdVertex - 1]; //idem
        g->adjacencyLists[edge.beginIdVertex - 1] = adj; //idem
    }
}

void addInverseEdge(Graph *g, Edge edge){
    bool doublonFound = false;
    adjacencyListElement *doublon = g->adjacencyLists[edge.endIdVertex - 1]; 
    while (doublon != NULL){
        if (doublon->v == edge.beginIdVertex - 1 && doublon->weight != edge.weight){
            doublon->weight = edge.weight;
            doublonFound = true;
            break;
        }
        doublon = doublon->next; 
    }

    if (!doublonFound){
        adjacencyListElement* adj = (adjacencyListElement *)malloc(sizeof(adjacencyListElement));
        if (adj == NULL){exit(0);}
        adj->v = edge.beginIdVertex - 1; //on enlève 1 pour matcher avec le format des fichiers pmed
        adj->weight = edge.weight;
        adj->next = g->adjacencyLists[edge.endIdVertex - 1]; //idem
        g->adjacencyLists[edge.endIdVertex - 1] = adj; //idem
    }
}


Graph* initializeGraph(char const *nom_instance){
    
    Graph *g = (Graph*)malloc(sizeof(*g));
    FILE *f = fopen(nom_instance, "r");
    if (f != NULL)
    {
        int nbVertices, nbEdge, p;
        fscanf(f, "%d %d %d\n", &nbVertices, &nbEdge, &p);
        Edge *edgeTable = (Edge *)malloc(sizeof(Edge) * nbEdge);
        int currentNbEdge = 0;

        g -> adjacencyLists = (adjacencyListElement**)malloc(nbVertices*sizeof(adjacencyListElement*));
        if (g-> adjacencyLists == NULL){exit(0);}
        g -> nbVertices = nbVertices;
        g -> p = p;
        g -> adom = nbVertices;
        g -> dom = (int*)malloc(nbVertices*sizeof(int));
        g -> save = (int*)malloc(nbVertices*sizeof(int));
        g -> branched = (int*)malloc(nbVertices*sizeof(int));
        g -> ingraph = (int*)malloc(nbVertices*sizeof(int));
        for (int i = 0; i < nbVertices; i++){
            g -> adjacencyLists[i] = NULL;
            g -> dom[i]=0;
            g -> save[i]=0;
            g -> branched[i]=0;
            g -> ingraph[i]=1;
        }

        for (int i = 0; i < nbEdge; i++){
            
            Edge edge;
            fscanf(f, "%d %d %d\n", &edge.beginIdVertex, &edge.endIdVertex, &edge.weight);

            addEdge(g, edge);
            addInverseEdge(g, edge);

            edgeTable[currentNbEdge] = edge;
            currentNbEdge++;
            
        }
        fclose(f);
        free(edgeTable);
    }
    floydWarshall(g);
    return g;
}

/*Graph* initializeGraphTsp(char const *nom_instance, int p){
    
    Graph *g = (Graph*)malloc(sizeof(*g));
    FILE *f = fopen(nom_instance, "r");
    int nbVertices;
    Coord *coordTab;

    if (f != NULL)
    {
        for(int i = 0; i < 3; i++){
            fscanf(f, "%*[^\n]\n");
        }
        
        fscanf(f, "DIMENSION : %d\n", &nbVertices);
        fscanf(f, "%*[^\n]\n");
        fscanf(f, "%*[^\n]\n");

        g -> adjacencyLists = (adjacencyListElement**)malloc(nbVertices*sizeof(adjacencyListElement*));
        if (g-> adjacencyLists == NULL){exit(0);}
        g -> nbVertices = nbVertices;
        g -> p = p;

        coordTab = (Coord *)malloc(nbVertices * sizeof(Coord));

        for (int i = 0; i < nbVertices; i++){
            g -> adjacencyLists[i] = NULL;
            Coord coord;
            fscanf(f, "%d %lf %lf\n", &coord.idVertex, &coord.x, &coord.y);
            coordTab[i] = coord;
        }
        fclose(f);
    }

    int** distanceMatrix = (int **)malloc(g->nbVertices * sizeof(int*)); //distanceMatrix[sdeb][sfin]
    if (distanceMatrix == NULL){exit(0);}
    //Initialisation
    for (int i = 0; i < g->nbVertices; i++){
        int* d = (int *)malloc(g->nbVertices * sizeof(int));
        distanceMatrix[i] = d;
    }
    g->distanceMatrix = distanceMatrix;
    Edge edge;
    for (int i = 0; i < nbVertices; i++){
        g->distanceMatrix[i][i] = 0;
        for (int j = i + 1; j < nbVertices; j++){
            edge.beginIdVertex = i + 1;
            edge.endIdVertex = j + 1;
            edge.weight = (int)sqrt(pow(coordTab[j].x - coordTab[i].x, 2.0) + pow(coordTab[j].y - coordTab[i].y, 2.0));
            addEdge(g, edge);
            addInverseEdge(g, edge);
            g->distanceMatrix[i][j] = edge.weight;
            g->distanceMatrix[j][i] = edge.weight;
        }
    }
    free(coordTab);
        
    return g;
}*/

void free_graph(Graph *g){
    int n = g->nbVertices;
    for (int i = 0; i < n; i++){
        adjacencyListElement *el = g -> adjacencyLists[i];
        while (el != NULL){
            adjacencyListElement *temp = el;
            el = el->next;
            free(temp);
        }
        free(g->distanceMatrix[i]);
    }
    free(g->adjacencyLists);
    free(g->distanceMatrix);
    free(g);
}

void afficherGraph(Graph *g){
    int z;
    for (int i = 0; i < g->nbVertices; i++){
        printf("%d", i);
        adjacencyListElement *adj = g->adjacencyLists[i];
        z=0;
        while (adj != NULL){
            printf(" -> (%d, %d)", adj->v, adj->weight);
            adj = adj->next;
            z++;
        }
        printf(" %d", z);
        printf("\n");
    }
}


void afficherDistanceMatrix(Graph *g){
    for (int i = 0; i < g->nbVertices; i++){
        printf("%d -> \t", i);
        for (int j = 0; j < g->nbVertices; j++){
            printf("%d\t", g->distanceMatrix[i][j]);
        }
        printf("\n");
    }
}


/* FLOYD WARSHALL */

void floydWarshall(Graph *g){
    int** distanceMatrix = (int **)malloc(g->nbVertices * sizeof(int*)); //distanceMatrix[sdeb][sfin]
    if (distanceMatrix == NULL){exit(0);}
    //Initialisation
    for (int i = 0; i < g->nbVertices; i++){
        int* d = (int *)malloc(g->nbVertices * sizeof(int));
        for (int j = 0; j < g->nbVertices; j++){
            d[j] = INT_MAX;
        }
        adjacencyListElement *adj = g->adjacencyLists[i];
        while (adj != NULL){
            d[adj->v] = adj->weight;
            adj = adj->next;
        }
        d[i] = 0;
        distanceMatrix[i] = d;
    }
    for (int k = 0; k < g->nbVertices; k++){
         for (int i = 0; i < g->nbVertices; i++){
            for (int j = 0; j < g->nbVertices; j++){
                if (distanceMatrix[i][k] != INT_MAX && distanceMatrix[k][j] != INT_MAX && distanceMatrix[i][j] > distanceMatrix[i][k] + distanceMatrix[k][j]){
                     distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                }
            }
        }
    }
    g->distanceMatrix = distanceMatrix;
}

/* FIN FLOYD WARSHALL */


int costPCenter(Graph *g, bool pcenters[]){
    //pcenters[i] = true if i is a center, otherwise false
    int cost = 0;
    for (int i = 0; i < g->nbVertices; i++){
        int min = INT_MAX;
        if (!pcenters[i]){
            for (int j = 0; j < g->nbVertices; j++){
                if (pcenters[j] && g->distanceMatrix[i][j] < min){
                    min = g->distanceMatrix[i][j];
                }
            }
            if (min > cost){
                cost = min;
            }
        }
    }
    return cost;
}
