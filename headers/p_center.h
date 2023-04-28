#ifndef p_center
#define p_center

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct Edge Edge;

struct Edge{
    int beginIdVertex; 
    int endIdVertex; 
    int weight;
};

typedef struct Coord Coord;

struct Coord{
    int idVertex; 
    double x; 
    double y;
};

typedef struct adjacencyListElement adjacencyListElement;

struct adjacencyListElement{
    int v;
    int weight;
    adjacencyListElement* next;
};

typedef struct Graph Graph;

struct Graph{
    int nbVertices;
    int p;
    int* dom;
    int* save;
    int* branched;
    adjacencyListElement** adjacencyLists;
    int** distanceMatrix;
};

void afficherGraph(Graph* g);
void addEdge(Graph *g, Edge edge);
void addInverseEdge(Graph *g, Edge edge);
Graph* initializeGraph(char const *nom_instance);
Graph* initializeGraphTsp(char const *nom_instance, int p);
void free_graph(Graph *g);
void afficherDistanceMatrix(Graph *g);
void floydWarshall(Graph *g);
int costPCenter(Graph *g, bool pcenters[]);
#endif
