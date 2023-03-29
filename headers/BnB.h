#include "p_center.h"
#include "misc.h"
#define UNUSED(x) (void)(x)


adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *U, adjacencyListElement *Dnow);
adjacencyListElement *ReduceBranches(Graph *g, adjacencyListElement *D, adjacencyListElement *U, adjacencyListElement *Dnow);