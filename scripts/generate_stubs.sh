#!/bin/bash

for module_name in *; do
    echo "Generating stubgen for $module_name"
    stubgen -p $module_name --include-docstrings  --inspect-mode
    if [ -d "./out/$module_name/" ]; then
      rsync -a ./out/$module_name/ ./$module_name/
    fi
    rm -rf ./out
done
