#!/bin/bash

usage() {
    echo "Usage: generate_stubs.sh <sofa-python3-site-packages-dir> "
    echo "This script automatically generates stub files for SofaPython3 modules"
}

if [ "$#" -eq 1 ]; then
    WORK_DIR=$1
else
    usage; exit 1
fi

cd $WORK_DIR


for module_name in *; do
    echo "Generating stubgen for $module_name"
    stubgen -v -p $module_name --include-docstrings  --inspect-mode --ignore-errors
    if [ -d "./out/$module_name/" ]; then
      rsync -a ./out/$module_name/ ./$module_name/
    fi
    rm -rf ./out
done
