instances="instances"

for fichier in "$instances"/*; do
    scp $fichier amenet@login.matrics.u-picardie.fr:instances
done