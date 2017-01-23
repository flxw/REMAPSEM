#!/bin/bash

# RUN MANUALLY BEFORE: ulimit -H -m $CONFIGURATION_MAX

source ./benchmark-configuration.sh
EXECUTABLE="./stream_c.exe"
config=$CONFIGURATION_MAX

while ((config>=CONFIGURATION_MIN)); do
  echo $(($config*1024))
  #ulimit -H -m $(($config*1024))
  #$EXECUTABLE | grep --after-context=3 'Copy:' | sed -r s/^/"$config "/g | sed s/://g | sed -r s/\\s+/,/g
  ((config=config-CONFIGURATION_STEP))
done
