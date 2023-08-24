# Generating the sphinx documentation

The sphinx documentation is automatically built by readthedocs.io when a commit is
added to the master branch. This readme describes the steps to locally build the
documentation on your PC. It is written for Ubuntu 20.04, but the steps are very
similar with other Linux distributions.

### 1. Installing the requirements
The following dependencies are required:

```shell
sudo apt install build-essential python3 python3-pip python3-sphinx
pip3 install --user sphinx_rtd_theme
```

Go inside the `SofaPython3/docs/sphinx` directory and run

```shell
pip3 install --user -r source/requirements.txt
```

### 2. Generating the docs
Make sure python can find SofaPython3 modules, and that these binary modules can find sofa dynamics libraries i.e.:
```shell
export PYTHONPATH="$SOFA_ROOT/plugins/SofaPython3/lib/python3/site-packages"
export SOFA_ROOT=/opt/sofa_v20.12/build/install
```

The following line can be used to make sure Sofa bindings can be found by python:
```shell
python -c "import importlib;print('OK') if importlib.util.find_spec('Sofa') else print('NOT OK');"
```

Then run: 
```shell
sphinx make html
```
