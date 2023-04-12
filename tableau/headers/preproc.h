#include "p_center.h"
#include "misc.h"

int bestToChoose(Graph *gd);
int undomBy(Graph *g, int x);
void domine(int x, Graph *gd);
void domineliste(int *sol, Graph *g);
int count(Graph *gd, int x);
void afficheDom(Graph *gd);
void afficheBranched(Graph *gd);
void unDom(Graph *g);
void createN1(Graph *gd, int x, int **N1);
void createN2(Graph *gd, int x, int **N1, int **N2);
void createN3(Graph *gd, int x, int **N1, int **N2, int **N3);
int nullTab(int *tab, int taille);
void reduceGraph(Graph *gd, int x);
adjacencyListElement * undomlist(Graph *gd);
void branchedf(Graph *g, int *df);
int ** initMatC(int taille);
void freeNs(int taille, int ** N1,int ** N2,int ** N3);