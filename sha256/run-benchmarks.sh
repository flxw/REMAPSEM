#!/bin/bash

EXECUTABLE=$1

for ((i=0; i<500; i+=50)); do
  echo "vvvvvvvvvvvvvvvvvvvvv"
  /usr/bin/time build/$EXECUTABLE ~/$i.zeroes
  echo "^^^^^^^^^^^^^^^^^^^^^"
done
