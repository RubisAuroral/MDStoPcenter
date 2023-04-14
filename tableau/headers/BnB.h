#include "p_center.h"
#include "misc.h"
#include "../headers/preproc.h"

typedef struct Branche Branche;

struct Branche{
    int * B;
};

adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *Dnow);
adjacencyListElement * BnB2(Graph *g);