#!/bin/sh

echo "into aging"
source ${EMX_SCRIPTS_DIR}/function.sh

sleep 10
run EmxAgeingServer -b

echo "leave aging"
