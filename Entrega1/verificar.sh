#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Utilização:' $0 'número_de_estudante'
    exit 0
fi


CFILES=$(echo -e "ficha2-5.c\nficha2-11.c\nficha2-12.c\nsinais-iii.c\nsinais-v.c")

OIFS="$IFS"
IFS=$'\n'

for file in $CFILES
do 
  ls ${file} &> /dev/null || echo Ficheiro \"$file\" não encontrado.
  execfile=${file%.c}
  ls ${execfile} &> /dev/null && ( grep  "$1" ${execfile} &>/dev/null || echo Verifique se faz a impressão solicitada em \"${file}\") || echo Ficheiro \"$execfile\" não encontrado.
done

IFS="$OIF"
