#include "p_center.h"
#include "misc.h"
#include "preproc.h"
#include <unistd.h>

typedef struct Branche Branche;

struct Branche{
    int * B;
    int x;
};

typedef struct iteratif iteratif;

struct iteratif{
    Branche etage;
    int last;
};

void BnB(Graph *gd);
void removeConflict(Graph *g, int *IS, int sommet);
void IkNeighbors(Graph* g, int * ISv, int * IS);
