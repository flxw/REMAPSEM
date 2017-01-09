#!/bin/bash

EXECUTABLE=$1

for ((i=0; i<1500; i+=50)); do
  echo -n "$i,"
  /usr/bin/time --format %E build/$EXECUTABLE ~/$i.zeroes > /dev/null
done
