#!/bin/bash

usage() {
    echo "Usage: generate_stubs.sh <destination> <module_name> "
    echo "This script automatically generates stub files for SofaPython3 modules"
}

if [ "$#" -ge 2 ]; then
    WORK_DIR=$1
else
    usage; exit 1
fi

destination_dir=$1
if [ ! -d "$destination_dir/$module_name/" ]; then
    echo "Destination directory does not contains a module with name $module_name"
    exit 1
fi


for module_name in "${@:2}"
do
    echo "Generating stubgen for $module_name in $destination_dir"
    pybind11-stubgen $module_name -o out
    if [ -d "./out/$module_name/" ]; then
      rsync -a ./out/$module_name/ $destination_dir/$module_name/
    fi
    echo $"Resync terminated for copying '$module_name' in '$destination_dir/$module_name/'"
    rm -rf ./out/$module_name
done
rm -rf "./out"