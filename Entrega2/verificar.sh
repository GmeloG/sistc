#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Utilização:' $0 'número_de_estudante'
    exit 0
fi


CFILES=$(echo -e "ex2_clt.c\nex2_srv.c\nex3_clt.c\nex3_srv.c\nex4_clt.c\nex4_srv.c\nex5_clt.c\nex5_srv.c\nex8a.c\nex8b.c")

OIFS="$IFS"
IFS=$'\n'

for file in $CFILES
do 
  ls ${file} &> /dev/null || echo Ficheiro \"$file\" não encontrado.
  execfile=${file%.c}
  ls ${execfile} &> /dev/null && ( grep  "$1" ${execfile} &>/dev/null || echo Verifique se faz a impressão solicitada em \"${file}\") || echo Ficheiro \"$execfile\" não encontrado.
done

IFS="$OIF"
