#!/bin/bash

FILES=( "sha256" "vsha256" )

for FILE in "${FILES[@]}"; do
  gcc -c -g $FILE.c
  objdump -Sd $FILE.o > $FILE.objdump
  mv $FILE.objdump ..
  rm $FILE.o
done
