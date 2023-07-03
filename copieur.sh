instances="instances/pcentre"

for fichier in "$instances"/*; do
    scp $fichier amenet@login.matrics.u-picardie.fr:pcentre/
done
