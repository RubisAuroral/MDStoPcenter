#include "../headers/p_center.h"

void freeList(adjacencyListElement *node) {
  adjacencyListElement *temp;
  while (node != NULL) {
    temp = node;
    node = node->next;
    free(temp);
  }
}

void exemple(Graph *g){
	int i=0;
  Edge edge;
	edge.weight=0;
	FILE *f = fopen("exemple4f", "r");
    	while(fscanf(f, "%d %d", &edge.beginIdVertex, &edge.endIdVertex) !=  EOF){
        addEdge(g,edge);
    		addInverseEdge(g,edge);
        i++;
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
      g -> branched = (int*)malloc(nbVertices*sizeof(int));
    	g -> adjacencyLists = (adjacencyListElement**)malloc(nbVertices*sizeof(adjacencyListElement*));
        if (g-> adjacencyLists == NULL){exit(0);}
        g -> nbVertices = nbVertices;
        for (int i = 0; i < nbVertices; i++){
            g -> adjacencyLists[i] = NULL;
            g -> dom[i] = 0 ;
            g -> branched[i] = 0 ;
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
  for(int i=0; i<taille; i++) if(L[i]) count++;
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

void Intersection(int *tab, adjacencyListElement *Liste1, int *Liste2, int taille){
  adjacencyListElement * temp = Liste1;
  for(int i=0; i<taille; i++){
    if(tab[i]==1) if(!inL(Liste1,i,-1)) tab[i]==0;
  }
  freeList(temp);
}

void trierListe(Graph *g, adjacencyListElement** liste) {
    int trie = 0;
    while (!trie) {
        trie = 1;
        adjacencyListElement* current = liste;
        adjacencyListElement* previous = NULL;
        while (current != NULL && current->next != NULL) {
            if (undomBy(g,current->v) < undomBy(g,current->next->v)) {
                // Échange les valeurs des noeuds actuel et suivant
                int temp = current->v;
                current->v = current->next->v;
                current->next->v = temp;
                trie = 0;
            }
            previous = current;
            current = current->next;
        }
    }
}

void freeGraph(Graph *g){
  for(int i=0; i<g->nbVertices; i++) freeList(g->adjacencyLists[i]);
  free(g->adjacencyLists);
  free(g->branched);
  free(g->dom);
}