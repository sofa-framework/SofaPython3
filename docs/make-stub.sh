#!/bin/bash
## Generate the stubs for the C plugins for code inspection tools or sphinx/readthedocs.
## To make stubs: https://github.com/sizmailov/pybind11-stubgen with patch from SofaDefrost.
export PYTHONPATH=../../../builds/release/lib/site-packages/
pybind11-stubgen -o stubs Sofa 
pybind11-stubgen -o stubs SofaRuntime 
rm -rf sphinx\-stubs/*
mv stubs/Sofa-stubs sphinx\-stubs/Sofa
mv stubs/SofaRuntime-stubs sphinx\-stubs/SofaRuntime
find sphinx\-stubs -name '*.pyi' -exec sh -c 'mv "$0" "${0%.pyi}.py"' {} \;


