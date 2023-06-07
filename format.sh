#!/bin/sh

while read -r line; do
    for word in $line; do
        if [[ $word =~ ^[0-9]+$ ]]; then
            ((word++))
        fi
        echo -n "$word "
    done
    echo ""
done < "$1"