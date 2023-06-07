#include "../headers/p_center.h"

void freeList(adjacencyListElement *node) {
  adjacencyListElement *temp;
  while (node != NULL) {
    temp = node;
    node = node->next;
    free(temp);
  }
}

int nbVoisin(Graph *g, int x){
  adjacencyListElement *temp = g->adjacencyLists[x];
  int z=0;
  while(temp!=NULL){
    if(!g->dom[temp->v]) z++;
    temp=temp->next;
  }
  return z;
}

int nbVoisinv2(Graph *g, int x){
  adjacencyListElement *temp = g->adjacencyLists[x];
  int z=0;
  if(!g->dom[x]) z++;
  while(temp!=NULL){
    if(!g->dom[temp->v]) z++;
    temp=temp->next;
  }
  return z;
}

int degre(Graph *g, int x){
  adjacencyListElement *temp = g->adjacencyLists[x];
  int z=0;
  while(temp!=NULL){
    z++;
    temp=temp->next;
  }
  return z;
}

void exemple(Graph *g, char *file){
  Edge edge;
	edge.weight=0;
	FILE *f = fopen(file, "r");
    	while(fscanf(f, "%d %d", &edge.beginIdVertex, &edge.endIdVertex) !=  EOF){
        if(edge.beginIdVertex != edge.endIdVertex){
          addEdge(g,edge);
          addInverseEdge(g,edge);
          edge.weight++;
        }
    	}
      fclose(f);
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

void deleteNode(adjacencyListElement **L, int x) {
  if(*L==NULL) return;

  if((*L)->v == x){
    adjacencyListElement * del = *L;
    *L = (*L)->next;
    free(del);
    return;
  }

  for(adjacencyListElement * current = * L; current->next!=NULL; current=current->next){
    if(current->next->v==x){
      adjacencyListElement * del = current->next;
      current->next=current->next->next;
      free(del);
      return;
    }
  }
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
      g -> save = (int*)malloc(nbVertices*sizeof(int));
      g -> branched = (int*)malloc(nbVertices*sizeof(int));
      g -> ingraph = (int*)malloc(nbVertices*sizeof(int));
    	g -> adjacencyLists = (adjacencyListElement**)malloc(nbVertices*sizeof(adjacencyListElement*));
      g -> voisins = (int**) malloc(g->nbVertices * sizeof(int*));
      for (int i = 0; i < 3; i++) {
        g->voisins[i] = (int*) malloc(g->nbVertices * sizeof(int));
      }
      
        if (g-> adjacencyLists == NULL){exit(0);}
        g -> nbVertices = nbVertices;
        for (int i = 0; i < nbVertices; i++){
            g -> adjacencyLists[i] = NULL;
            g -> dom[i] = 0 ;
            g -> save[i] = 0 ;
            g -> branched[i] = 0 ;
            g -> ingraph[i] = 1 ;
        }
        return g;
}

int inL(adjacencyListElement *adj, int x, int base){
  adjacencyListElement *adj2 = adj;
  while(adj2 != NULL){
    if(adj2->v==x || x==base) return 1;
    adj2 = adj2->next;
  }
  freeList(adj2);
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

int listeSize(int *L, int taille){
  int count=0;
  for(int i=0; i<taille; i++) count+=L[i];
  return count;
}

int listeSize2(adjacencyListElement *L){
  int count = 0;
  adjacencyListElement *temp=L;
  while(temp!=NULL){
    count++;
    temp=temp->next;
  }
  return count; 
}

void Union(int * Liste1, adjacencyListElement * Liste2){
  adjacencyListElement * temp = Liste2;
  while(temp!=NULL){
    Liste1[temp->v]=1;
    temp=temp->next;
  }
  freeList(temp);
}

void Intersection(int *tab, adjacencyListElement *Liste1, int *Liste2){
  adjacencyListElement * temp = Liste1;
  while(temp!=NULL){
    if(!Liste2[temp->v]) tab[temp->v]=0;
    else tab[temp->v]=1;
    temp=temp->next;
  }
  freeList(temp);
}

void freeGraph(Graph *g){
  for(int i=0; i<g->nbVertices; i++) freeList(g->adjacencyLists[i]);
  free(g->adjacencyLists);
  free(g->branched);
  free(g->dom);
}