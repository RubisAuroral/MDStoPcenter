#include "p_center.h"
#include "misc.h"

int bestToChoose(Graph *gd);
int undomBy(Graph *g, int x);
void domine(int x, Graph *gd);
void domineliste(adjacencyListElement *df, Graph *g);
int count(Graph *gd, int x);
void afficheDom(Graph *gd);
void unDom(Graph *g);
adjacencyListElement * createN1(Graph *gd, int x);
adjacencyListElement * createN2(Graph *gd, int x, adjacencyListElement *L);
adjacencyListElement * createN3(Graph *gd, int x, adjacencyListElement *N1, adjacencyListElement *N2);
void reduceGraph(Graph *gd, int x);
adjacencyListElement * undomlist(Graph *gd);