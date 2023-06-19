#include "p_center.h"
#include "misc.h"

int bestToChoose(Graph *gd);
void domine(int x, Graph *gd);
void dominesave(int x, Graph *gd);
void domineliste(int *sol, Graph *g);
void alber(Graph *g, int * df);
int alldom(Graph *gd, int x);
void afficheDom(Graph *gd);
void afficheBranched(Graph *gd);
void createN1(Graph *gd, int x, int *N1);
void createN2(Graph *gd, int x, int *N1, int *N2);
void createN3(Graph *gd, int x, int *N1, int *N2, int *N3);
int nullTab(int *tab, int taille);
int fullTab(int * tab, int taille);
void reduceGraph(Graph *gd, int x);
void reduceGraphN3(Graph *gd, int x, int mem);
void simplerules(Graph *g);
void created0(Graph *g, int * d0);
void branchedf(Graph *g, int *df);
void unDom(Graph *g);