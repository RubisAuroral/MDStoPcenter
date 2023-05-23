#include "p_center.h"
#include "misc.h"
#include "preproc.h"
#include <unistd.h>

typedef struct Branche Branche;

struct Branche{
    Graph *g;
    int * B;
    int x;
};

typedef struct iteratif iteratif;

struct iteratif{
    Branche etage;
    int last;
};

adjacencyListElement * BnB(Graph *g, adjacencyListElement *D, adjacencyListElement *Dnow);
void BnB2(Graph *g);
int * BnB3();
void BnBtest();
