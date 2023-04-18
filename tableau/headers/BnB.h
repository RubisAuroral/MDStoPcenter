#include "p_center.h"
#include "misc.h"
#include "../headers/preproc.h"

typedef struct Branche Branche;

struct Branche{
    int * B;
    int x;
};

adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *Dnow);
void BnB2(Graph *g);
int * BnB3();