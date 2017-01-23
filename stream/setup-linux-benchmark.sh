#!/bin/bash

source ./benchmark-configuration.sh

#cgm create memory limitedrss

for amount in $(seq $CONFIGURATION_MIN $CONFIGURATION_STEP $CONFIGURATION_MAX); do
  cgcreate -g memory:limitedrss/limit$amount
  echo "$amount"M > /sys/fs/cgroup/memory/limitedrss/limit$amount/memory.limit_in_bytes
done
