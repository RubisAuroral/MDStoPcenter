from networkx.generators.random_graphs import erdos_renyi_graph
import sys
import numpy as np
import random

n = int(sys.argv[1])
p = int(sys.argv[2])
p = p/100
g = erdos_renyi_graph(n, p)
tab = np.zeros((n,))
print(n)

for u in range(0, n):
    for v in range(u,n):
        if g.has_edge(u,v):
            tab[u]=tab[u]+1
            tab[v]=tab[v]+1
            print(u, v)

for u in range(0, n):
    if tab[u]==0:
        a = random.randint(0,n-1)
        while a==u:
            a = random.randint(0,n-1)
        g.add_edge(u, a)
        tab[u]=tab[u]+1
        tab[a]=tab[a]+1
        print(u, a)

