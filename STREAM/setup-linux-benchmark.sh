#!/bin/sh

source ./benchmark-configuration.sh

cgm create memory limitedrss

for amount in "${CONFIGURATIONS[@]}"; do
  cgm create   memory limitedrss/limit$amount
  cgm setvalue memory limitedrss/limit$amount memory.limit_in_bytes "$amount"M
done
