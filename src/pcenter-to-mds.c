#include "../headers/p_center.h"
#include "../headers/misc.h"
#include "../headers/preproc.h"
#include "../headers/BnB.h"
#include "../headers/pcenter-to-mds.h"

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

void mdsgraph(Graph *gtemp, Graph *origine, int dist){
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
            }
        }
    }
}