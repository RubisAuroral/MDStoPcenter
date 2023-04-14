#include "p_center.h"

void exemple(Graph *g);
void ajoute(adjacencyListElement** firstValue, int nouvelleValeur);
void afficheListe(adjacencyListElement * node);
void deleteNode(adjacencyListElement ** L, int x);
adjacencyListElement* difference(adjacencyListElement * L1, adjacencyListElement* L2);
Graph* cleanGraph(int x);
int inL(adjacencyListElement *adj, int x, int base);
int inLv2(adjacencyListElement *adj, adjacencyListElement *L);
int listeSize(int *L, int taille);
void Union(int * Liste1, adjacencyListElement * Liste2);
void Intersection(int *tab, adjacencyListElement *Liste1, int *Liste2, int taille);
void trierListe(Graph *g, adjacencyListElement** liste);
void freeList(adjacencyListElement *node);
void freeGraph(Graph *g);