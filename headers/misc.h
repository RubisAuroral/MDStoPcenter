#include "p_center.h"

void exemple(Graph *g);
void ajoute(adjacencyListElement** firstValue, int nouvelleValeur);
void afficheListe(adjacencyListElement * node);
adjacencyListElement * deleteNode(adjacencyListElement * L, int x);
adjacencyListElement* difference(adjacencyListElement * L1, adjacencyListElement* L2);
Graph* cleanGraph(int x);
int inL(adjacencyListElement *adj, int x, int base);
int inLv2(adjacencyListElement *adj, adjacencyListElement *L);
int listeSize(adjacencyListElement *L);
adjacencyListElement * Union(adjacencyListElement * Liste1, adjacencyListElement * Liste2);
adjacencyListElement * Intersection(adjacencyListElement *Liste1, adjacencyListElement *Liste2, int sommet);