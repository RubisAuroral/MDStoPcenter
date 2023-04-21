#!/bin/sh

if [ -z "$1" ]; then
  echo "Veuillez spécifier le nom du fichier à lire en argument."
  exit 1
fi

if [ ! -f "$1" ]; then
  echo "Le fichier spécifié n'existe pas."
  exit 1
fi

max=0

while read -r line; do
  for x in $line; do
    if [ "$x" -gt "$max" ]; then
      max="$x"
    fi
  done
done < "$1"

./main "16" "$1" "$max"