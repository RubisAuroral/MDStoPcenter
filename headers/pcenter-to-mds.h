#include "p_center.h"
#include "misc.h"
#include "preproc.h"

int distmax(Graph *g);
int distmin(Graph *g);
int *calcbornes(Graph *g);
int mdsgraph(Graph *gtemp, Graph *origine, int dist);
int dichotomie(Graph *g, int *tab, int sup, int inf);
int dichotomieChuMin(Graph *g, int *tab);