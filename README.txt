Compile:

make

Usage:

./main -f filename

Arguments possibles :

-k [1;+inf] : Le nombre d'IS entretenu - Défaut : 8
-d [0;100] : Le curseur de la dichotomie - Défaut : 50%
-dc : Effectue au préalable la dichotomieChuMin - Défaut : Non
-idc : Effectue au préalable la dichotomieChuMin - Défaut : Non
-i : Affiche toutes les informations - Défaut : Non

Exemple :

./main -f instances/pcentre/pmed8.txt -idc -d 33 -k 24

Lance le programme sur l'instance pmed8, avec la dichotomie inverse de Chu Min, le curseur de dichotomie à 33% et 24 IS
