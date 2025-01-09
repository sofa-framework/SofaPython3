# -*- coding: utf-8 -*-
"""
Autogenerate python wrapper for SOFA objects.

Contributors:
    damien.marchal@univ-lille.fr
"""
import re
import sys, os
import pprint
import argparse

import Sofa
import Sofa.Simulation

# some data field cannot have names as this is a python keywoards. 
reserved = ["in", "with", "for", "if", "def", "class", "global"]

def sofa_to_python_typename(name, short=False):
    t = {"string":"str",
         "bool": "bool",
         "TagSet" : "object",
         "BoundingBox" : "object",
         "ComponentState" : "object",
         "RGBAColor" : "object",
         "OptionsGroup" : "object",
         "SelectableItem" : "object",
         "Material" : "object",
         "DisplayFlags" : "object",
         "d" : "float",
         "f" : "float",
         "i" : "int",
         "I" : "int",
         "L" : "int",
         "l" : "int",
         "b" : "int",
         "LinkPath" : "LinkPath"
         }

    SofaArray = "TypeHints.SofaArray"
    if short:
        SofaArray = "TypeHints.SofaArray"

    if name in t:
        return t[name]
    elif "Rigid" in name:
        return SofaArray
    elif "vector" in name:
        return SofaArray
    elif "set" in name:
        return SofaArray
    elif "Vec" in name:
        return SofaArray
    elif "Mat" in name:
        return SofaArray 
    elif "Quat" in name:
        return SofaArray
    elif "map" in name:
        return "object"
    elif "fixed_array" in name:
        return SofaArray

    print(f"Warning: unknown type '{name}' encountered, falling back to generic object type")
    return "object"


def sofa_datafields_to_constructor_arguments_list(data_fields, object_name, has_template=True):
    #{'defaultValue': '0',
    #                 'group': '',
    #                 'help': 'if true, handle the events, otherwise ignore '
    #                         'the events',
    #                 'name': 'listening',
    #                 'type': 'bool'},
    required_data_fields = []
    optional_data_fields = []

    for data_field in data_fields:
        name = data_field["name"]
        if name in reserved:
            print(f"Warning: {object_name} contains a the data field named '{name}' which is also a python keyword")
            continue

        if " " in name:
            print(f"Warning: this is an invalid arguments name: '{name}'")
            continue

        if len(name) == 0:
            print("Warning: empty data field empty name")
            continue

        if data_field["isRequired"]:
            required_data_fields.append(data_field)
        else:
            optional_data_fields.append(data_field)

    result_params = ""
    if has_template:
        result_params = "template: Optional[str] = None, "
    result_params += ",".join([data["name"]+": "+sofa_to_python_typename(data["type"]) for data in required_data_fields])
    result_params += ",".join([data["name"]+": Optional["+sofa_to_python_typename(data["type"])+"] = None" for data in optional_data_fields])

    ordered_fields = sorted(data_fields, key=lambda x: x["name"])

    all = "\n           ".join( [ data["name"]+" ("+sofa_to_python_typename(data["type"], short=True)+"):   " + data["help"] for data in ordered_fields])
    all += "\n           template (str): the type of degree of freedom"
    return result_params, all

def sofa_datafields_to_doc(data_fields):
    p = ""
    for data in data_fields:
        name = data["name"]
        help = data["help"]
        if len(name) == 0:
            continue
        if " " in name:
            continue

        if name in reserved:
            p += "\t\t " + name + ": " + help + " (NB: use the kwargs syntax as name is a reserved word in python)\n\n"
        else:
            p += "\t\t " + name + ": " + help + "\n\n"

    return p

def clean_sofa_text(t):
    t = t.replace("\n","")
    t = t.replace("'", "")
    return t

def sofa_datafields_to_typehints(data_fields, mode="Sofa"):
    p = ""
    p2 = ""
    for data in data_fields:
        name = data["name"]
        help = clean_sofa_text(data["help"])
        type = sofa_to_python_typename(data["type"])
        if len(name) == 0:
            continue

        if " " in name:
            continue

        if name in reserved:
            p += "    " + name + ": " + help + " (NB: use the kwargs syntax as name is a reserved word in python)\n\n"
        else:
            p += f"    {name}: Data[{type}] \n    '{help}'\n\n"
            p2 += f"        {name}: Optional[{type} | LinkPath] = None \n        '{help}'\n\n"

    return p, p2

def make_all_init_files(root_dir):
    entries = {}
    for dirpath, dirnames, filenames in os.walk(root_dir):
        # Test if the root_dir and the dirpath are the same to skip first entry
        # This could be implemented by using [os.walk(root_dir)][1:] but in that case 
        # python type hints get lost on my version 3.10. Maybe in a future python release
        # this case will be handled
        if os.path.abspath(root_dir) == os.path.abspath(dirpath):
            continue 

        initfile = open(dirpath + "/__init__.py", "wt")
        res = []
        fres = []
        fres2 = []
        for d in dirnames:
            res.append(d)
        for f in filenames:
            if f.endswith(".py") and f != "__init__.py":
                fres.append(f[:-3])
                res.append(f[:-3])
        listc = ""
        for r in res:
            listc += "    " + str(r) + "\n"

        initfile.write("# -*- coding: utf-8 -*-\n\n")
        initfile.write("""
\"\"\"
Sofa Component %s 

Summary:
========

%s
    
\"\"\"  
""" % (os.path.basename(dirpath), listc))

def wrapper_code(class_name, description, data_list, properties_doc, 
                 class_typehints, params_typehints, 
                 constructor_params_typehints,
                 constructor_params_docs):
    properties_doc = ""
    return f"""    
class {class_name}(Object):
    \"\"\"{description}\"\"\"

    def __init__(self, {constructor_params_typehints}):
        \"\"\"{description}
        
        Args:
           {constructor_params_docs}
        \"\"\"
        ...

{class_typehints}

"""

def documentation_code(class_name):
    return """
\"\"\"
Component %s

.. autofunction:: %s

Indices and tables
******************

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
\"\"\"
import Sofa
from Sofa.Core import Object, LinkPath
from Sofa.Component.TypeHints import Data, Optional, SofaArray
import dataclasses
""" % (class_name, class_name)

def select_single_template(template_list):
    """ returns a single template"""
    for dim in ["Vec3", "Rigid3", "Vec2", "Rigid2", "Vec1", "Quatd"]:
        for template in template_list:
            if "Cuda" not in template and dim in template:
                return template

    for template in template_list:
        return template

    return ""


def create_typhint(pathname):
    from pathlib import Path
    path = Path(pathname).parent
    if not os.path.exists(str(path)):
        path.mkdir(parents=True)
    c="""
from typing import TypeVar, Generic, Optional as __optional__
import numpy.typing
from Sofa.Core import Node, Object, LinkPath
import numpy
from numpy.typing import ArrayLike

T = TypeVar("T", bound=object)

# This is a generic type 'T' implemented without PEP 695 (as it needs python 3.12)
class Data(Generic[T]):
    linkpath: LinkPath
    value: T

Optional = __optional__
SofaArray = numpy.ndarray | list
"""
    with open(pathname,"w") as w:
        w.write(c)

from pprint import pprint

def load_component_list(target_name):
    import json
    import Sofa
    import Sofa.Core
    import SofaRuntime
    
    if target_name in ["Sofa"]:
        # The binding is not a sofa plugin. 
        print("Loading a python module")
    else:
        print("Loading a sofa plugin")
        SofaRuntime.importPlugin(target_name)

    json = json.loads(Sofa.Core.ObjectFactory.dump_json())

    selected_entries = []
    for item in json:
        selected_item = None
        for type, entry in item["creator"].items():
            if entry["target"].startswith(target_name):
                for data in entry["object"]["data"]: 
                    data["isRequired"] = True
                selected_item = item

        if selected_item:
            selected_entries.append(selected_item)
    
    print("Number of objects ", len(json))
    print("Number of selected objects ", len(selected_entries))

    return selected_entries

def create_sofa_stubs(code_model, target_path):
    blacklist = ["RequiredPlugin"]

    template_nodes = dict()
    not_created_objects = list()

    create_typhint(target_path + "Sofa/Component/TypeHints.py")

    for entry in code_model:
        data = {}

        class_name = entry["className"]
        entry_templates = [n for n in entry["creator"].keys()]
        description = entry["description"]
        
        selected_template = select_single_template(entry_templates)
        selected_entry = entry["creator"][selected_template]
        selected_class = selected_entry["class"]
        selected_object = selected_entry["object"]
        selected_target = selected_entry["target"]

        if len(selected_target) == 0:
            selected_target = "unknown_target"

        object_name = class_name + "<" + selected_template + ">"
        if object_name in blacklist or class_name in blacklist:
            print("Skipping ", object_name)
            continue


        data = selected_object["data"]   # obj.getDataFields()
        links = selected_object["link"]
        target = selected_target

        #load_existing_stub(selected_target, class_name)
            
        arguments_list, constructor_params_doc = sofa_datafields_to_constructor_arguments_list(data, object_name, len(entry_templates) > 0)
        params_doc = sofa_datafields_to_doc(data)
        class_typehint, params_typehint = sofa_datafields_to_typehints(data)
        code = wrapper_code(class_name, description.strip(), arguments_list, params_doc, 
                                class_typehint, params_typehint, 
                                arguments_list, constructor_params_doc)

        pathname = target_path + target.replace(".","/") + "/"
        full_component_name = target + "." + class_name

        # Creates the destination directory if needed. 
        os.makedirs(pathname, exist_ok=True)

        outfile = open(pathname + class_name + ".py", "wt")
        outfile.write("# -*- coding: utf-8 -*-\n\n")
        outfile.write(documentation_code(class_name))
        outfile.write(code)
        outfile.close()

        if full_component_name in code_model:
            raise Exception("Already existing entry")

    # In every directory, scan the object that are in and generates an init.py file 
    make_all_init_files(target_path)
    return code_model

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
                    prog='sofa-component-stub-generator',
                    description='Generates python stubs that describes sofa components that have no binding')
    parser.add_argument('--target_name', default="Sofa.Component")  
    parser.add_argument('--output_directory', default="out/")  
    args = parser.parse_args()

    target_name = args.target_name
    output_directory = args.output_directory

    print(f"Generating SOFA's components python interfaces for {target_name}")

    components = load_component_list(target_name)
    create_sofa_stubs(components, output_directory)
    
