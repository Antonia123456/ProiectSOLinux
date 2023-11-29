#!/bin/bash


char="$1"
count=0 

if [ "$#" -ne 1 ]; then
    echo "Nr incorect arg"  
    exit 1
fi

function verif 
{
    linie="$1" 
    
    if [[ ! "$linie" =~ ^[A-Z] ]]; then return 1 
    fi

    if [[ "$linie" =~ $char && ! "$linie" =~ ,.*[[:space:]]si && "$linie" =~ ^[A-Za-z0-9\ ,.!]+[?!.,]$ ]];   
          then return 0 
    fi

    return 1 
}

while IFS= read -r linie || [ -n "$linie" ]; do
    if verif "$linie"; then ((count++))  
    fi
done

echo $count 
