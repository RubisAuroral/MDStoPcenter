#include "../headers/p_center.h"


/*Non utilisé : Initialise un tableau à 0*/
void inittab(int *tab, int size){
  for(int i=0; i<size; i++){
    tab[i]=0;
  }
}

/*Free une liste*/
void freeList(adjacencyListElement *node) {
  adjacencyListElement *temp;
  while (node != NULL) {
    temp = node;
    node = node->next;
    free(temp);
  }
}

/*Retourne le nomnbre de voisin non dominé*/
int nbVoisin(Graph *g, int x){
  adjacencyListElement *temp = g->adjacencyLists[x];
  int z=0;
  while(temp!=NULL){
    if(!g->dom[temp->v]) z++;
    temp=temp->next;
  }
  return z;
}

/*Retourne le nomnbre de voisin non dominé en se comptant lui même comme un voisin*/
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

/*Retourne le degré d'un sommet*/
int degre(Graph *g, int x){
  adjacencyListElement *temp = g->adjacencyLists[x];
  int z=0;
  while(temp!=NULL){
    z++;
    temp=temp->next;
  }
  return z;
}

/*Fonction de test plus utilisé.*/
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

/*Ajoute un élément à une liste*/
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

/*Affiche le contenu d'une liste*/
void afficheListe(adjacencyListElement * node) {
  while (node != NULL) {
    printf("%d ", node->v+1);
    node = node->next;
  }
  printf("\n");
}

/*Supprime un sommet d'une liste*/
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

/*Obsolèté : Renvoie la liste L1 de laquelle on a retiré tout éléments de la liste L2*/
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

/*Génère & initialise un graphe vierge*/
Graph* cleanGraph(int x){
	Graph *g = (Graph*)malloc(sizeof(*g));
      int nbVertices=x;
    	g -> dom = (int*)malloc(nbVertices*sizeof(int));
      g -> save = (int*)malloc(nbVertices*sizeof(int));
      g -> branched = (int*)malloc(nbVertices*sizeof(int));
      g -> ingraph = (int*)malloc(nbVertices*sizeof(int));
    	g -> adjacencyLists = (adjacencyListElement**)malloc(nbVertices*sizeof(adjacencyListElement*));
      
        if (g-> adjacencyLists == NULL){exit(0);}
        g -> nbVertices = nbVertices;
        g -> adom = nbVertices;
        for (int i = 0; i < nbVertices; i++){
            g -> adjacencyLists[i] = NULL;
            g -> dom[i] = 0 ;
            g -> save[i] = 0 ;
            g -> branched[i] = 0 ;
            g -> ingraph[i] = 1 ;
        }
        return g;
}

/*Obsolète : Renvoie si un sommet est présent dans une liste*/
int inL(adjacencyListElement *adj, int x, int base){
  adjacencyListElement *adj2 = adj;
  while(adj2 != NULL){
    if(adj2->v==x || x==base) return 1;
    adj2 = adj2->next;
  }
  freeList(adj2);
  return 0;
}

/*Obsolète : Renvoie si une liste est inclues dans une autre liste*/
int inLv2(adjacencyListElement *adj, adjacencyListElement *L){
  int test=0;
  adjacencyListElement *adj2 = adj;
  while(adj2 != NULL && test==0){
    test=inL(L, adj2->v,-1);
    adj2 = adj2->next;
  }
  return test;
}

/*Renvoie la taille d'un tableau)*/
int listeSize(int *L, int taille){
  int count=0;
  for(int i=0; i<taille; i++) count+=L[i];
  return count;
}

/*Obsolète : Renvoie la taille d'une liste*/
int listeSize2(adjacencyListElement *L){
  int count = 0;
  adjacencyListElement *temp=L;
  while(temp!=NULL){
    count++;
    temp=temp->next;
  }
  return count; 
}

/*Obsolète : Renvoie l'union de deux listes*/

void Union(int * Liste1, adjacencyListElement * Liste2){
  adjacencyListElement * temp = Liste2;
  while(temp!=NULL){
    Liste1[temp->v]=1;
    temp=temp->next;
  }
  freeList(temp);
}

/*Obsolète : Renvoie l'intersection de deux listes*/
void Intersection(int *tab, adjacencyListElement *Liste1, int *Liste2){
  adjacencyListElement * temp = Liste1;
  while(temp!=NULL){
    if(!Liste2[temp->v]) tab[temp->v]=0;
    else tab[temp->v]=1;
    temp=temp->next;
  }
  freeList(temp);
}

/*Affiche le statut du graphe*/
void print_graph_status(Graph *g){
  printf("Nombre se sommets : %d\n", g->nbVertices);
  printf("Sommets du graph : ");
  for(int i=0; i<g->nbVertices; i++) printf("%d", g->ingraph[i]);
  printf("\nEtat branché : ");
  for(int i=0; i<g->nbVertices; i++) printf("%d", g->branched[i]);
  printf("\nEtat dominé : ");
  for(int i=0; i<g->nbVertices; i++) printf("%d", g->dom[i]);
  printf("\nDominés par défaut : ");
  for(int i=0; i<g->nbVertices; i++) printf("%d", g->save[i]);
  printf("\n");
}

/*Affiche un graphe formaté dans le format des instances de Hua*/
void instanceHua(Graph *g){
  int nbedge=0;
  for(int i=0; i<g->nbVertices; i++){
    adjacencyListElement *temp = g->adjacencyLists[i];
    while(temp!=NULL){
      nbedge++;
      temp=temp->next;
    }
  }
  printf("p edge %d %d", g->nbVertices, nbedge);
  for(int i=0; i<g->nbVertices; i++){
    adjacencyListElement *temp = g->adjacencyLists[i];
    while(temp!=NULL){
      if(temp->v > i) printf("\ne %d %d", i+1, temp->v+1);
      temp=temp->next;
    }
  }
}

/*Free le graphe*/
void freeGraph(Graph *g){
  for(int i=0; i<g->nbVertices; i++) freeList(g->adjacencyLists[i]);
  free(g->adjacencyLists);
  free(g->branched);
  free(g->dom);
  free(g->save);
  free(g->ingraph);
}