#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Chyba: Skript vyžaduje dva argumenty - první je jméno souboru s definicí hracího pole pro hru Life a druhý je počet iterací."
    exit 1
fi

source_file="life.cpp"
input_file="$1"
iterations="$2"

mpic++ --prefix /usr/local/share/OpenMPI -o life life.cpp || { echo "Chyba: Překlad se nezdařil."; exit 1; }

mpirun --prefix /usr/local/share/OpenMPI --oversubscribe -np 3 life  $input_file $iterations

rm -f life

exit 0

