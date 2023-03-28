#include "../headers/p_center.h"

void exemple(Graph *g){
	Edge edge;
	edge.weight=0;
	FILE *f = fopen("exemple", "r");
    	while(fscanf(f, "%d %d", &edge.beginIdVertex, &edge.endIdVertex) !=  EOF){
    		addEdge(g,edge);
    		addInverseEdge(g,edge);
    	}
	printf("\n");
}

void ajoute(adjacencyListElement** firstValue, int nouvelleValeur) {
  adjacencyListElement *element = (adjacencyListElement*) malloc(sizeof(adjacencyListElement));
  adjacencyListElement *last = *firstValue;

  element->v = nouvelleValeur;
  element->next = NULL;

  if (*firstValue == NULL) {
     *firstValue = element;
     return;
  }

  while (last->next != NULL) {
    last = last->next;
  }

  last->next = element;
  return;
}


void afficheListe(adjacencyListElement * node) {
  while (node != NULL) {
    printf("%d ", node->v+1);
    node = node->next;
  }
  printf("\n");
}

adjacencyListElement * deleteNode(adjacencyListElement *L, int x) {
	adjacencyListElement *temp = L;
	adjacencyListElement *new = NULL;
	while(temp!=NULL){
	  if(temp->v!=x) ajoute(&new, temp->v);
		temp=temp->next;
	}
	return new;
}

adjacencyListElement* difference(adjacencyListElement * L1, adjacencyListElement* L2) {
  adjacencyListElement *temp, *result = NULL;
  int found;

  while (L1 != NULL) {
    temp = L2;
    found = 0;
    while (temp != NULL) {
      if (L1->v == temp->v) {
        found = 1;
        break;
      }
      temp = temp->next;
    }
    if (found == 0) {
      ajoute(&result, L1->v);
    }
    L1 = L1->next;
  }
  return result;
}

Graph* cleanGraph(int x){
	Graph *g = (Graph*)malloc(sizeof(*g));
    	int nbVertices=x;
    	g -> dom = (int*)malloc(nbVertices*sizeof(int));
    	g -> adjacencyLists = (adjacencyListElement**)malloc(nbVertices*sizeof(adjacencyListElement*));
        if (g-> adjacencyLists == NULL){exit(0);}
        g -> nbVertices = nbVertices;
        for (int i = 0; i < nbVertices; i++){
            g -> adjacencyLists[i] = NULL;
            g -> dom[i] = 0 ;
        }
        return g;
}

int inL(adjacencyListElement *adj, int x, int base){
  adjacencyListElement *adj2 = adj;
  while(adj2 != NULL){
    if(adj2->v==x || x==base) return 1;
    adj2 = adj2->next;
  }
  return 0;
}

int inLv2(adjacencyListElement *adj, adjacencyListElement *L){
  int test=0;
  adjacencyListElement *adj2 = adj;
  while(adj2 != NULL && test==0){
    test=inL(L, adj2->v,-1);
    adj2 = adj2->next;
  }
  return test;
}

int listeSize(adjacencyListElement *L){
  adjacencyListElement *temp = L;
  int count=0;
  while(temp!=NULL){
    count++;
    temp=temp->next;
  }
  return count;
}