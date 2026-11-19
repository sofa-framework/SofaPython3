import sys
import os
import shutil
from sofaStubgen import load_component_list, create_sofa_stubs, find_bound_modules
from mypy.stubgen import parse_options, generate_stubs
import argparse




#Method to use pybind11-stubgen
def pybind11_stub(module_name: str):
    import logging
    from pybind11_stubgen import CLIArgs, stub_parser_from_args, Printer, to_output_and_subdir, run, Writer
    logging.basicConfig(
        level=logging.INFO,
        format="%(name)s - [%(levelname)7s] %(message)s",
    )
    args = CLIArgs(
        module_name=module_name,
        output_dir='./out',
        stub_extension="pyi",
        # default ags:
        root_suffix=None,
        ignore_all_errors=False,
        ignore_invalid_identifiers=None,
        ignore_invalid_expressions=None,
        ignore_unresolved_names=None,
        exit_code=False,
        numpy_array_wrap_with_annotated=False,
        numpy_array_use_type_var=False,
        numpy_array_remove_parameters=False,
        enum_class_locations=[],
        print_safe_value_reprs=None,
        print_invalid_expressions_as_is=False,
        dry_run=False)

    parser = stub_parser_from_args(args)
    printer = Printer(invalid_expr_as_ellipses=not args.print_invalid_expressions_as_is)


    run(
        parser,
        printer,
        output_dir=args.output_dir,
        module_names=[args.module_name],
        root_suffix=args.root_suffix,
        dry_run=args.dry_run,
        writer=Writer(stub_ext=args.stub_extension),
    )

# This class is used with shutil.copytree to skip already existing files in dest folder.
class noOverrideFilter:
    def __init__(self,fromFolder,destFolder):
        self.fromFolder = fromFolder
        self.destFolder = destFolder

    def findDestPath(self,currentRelativeToFromPath):

        absoluteFromFolder = os.path.abspath(self.fromFolder)

        #here we remove the first folder because it is the output folder
        destPath = ''
        head = os.path.abspath(currentRelativeToFromPath)
        while(head != absoluteFromFolder):
            head,tail = os.path.split(head)
            destPath = os.path.join(destPath,tail)

        return os.path.abspath(os.path.join(self.destFolder,destPath))


    def __call__(self,path, objectList):

        destPath = self.findDestPath(path)
        returnList = []
        for obj in objectList:
            if(os.path.isfile(os.path.join(destPath,obj))):
                returnList.append(obj)
        return returnList

# pybind11-stubgen can write imports (e.g. 'import Sofa.Core') before 'from __future__ import annotations',
# which is a SyntaxError: move the __future__ imports right after the module docstring.
def fix_future_imports(stub_path):
    import io
    import tokenize

    with open(stub_path, encoding="utf-8") as f:
        source = f.read()
    lines = source.splitlines(keepends=True)

    # index of the first line after the module docstring (0 if there is none)
    insert_at = 0
    try:
        for token in tokenize.generate_tokens(io.StringIO(source).readline):
            if token.type in (tokenize.ENCODING, tokenize.NL, tokenize.NEWLINE, tokenize.COMMENT):
                continue
            if token.type == tokenize.STRING:
                insert_at = token.end[0]
            break
    except (tokenize.TokenError, SyntaxError):
        return

    future_lines = [i for i, line in enumerate(lines)
                    if i >= insert_at and line.startswith("from __future__ import")]
    if not future_lines:
        return

    future = [line if line.endswith("\n") else line + "\n" for i, line in enumerate(lines) if i in future_lines]
    others = [line for i, line in enumerate(lines) if i not in future_lines]
    fixed = "".join(others[:insert_at] + future + others[insert_at:])
    if fixed != source:
        with open(stub_path, "w", encoding="utf-8") as f:
            f.write(fixed)

def fix_stubs_in_directory(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(".pyi"):
                fix_future_imports(os.path.join(root, file))

#Generate stubs using either pybind11-stubgen or mypy version of stubgen
def generate_module_stubs(module_name, work_dir, usePybind11_stubgen = False):
    print(f"Generating stubgen for {module_name} in {work_dir}")

    if(usePybind11_stubgen):
        #Use pybind11 stubgen
        #Could be replaced by an os call to
        #subprocess.run(["pybind11-stubgen", module_name, "-o", "out"], check=True)
        pybind11_stub(module_name)
    else:
        #Use mypy stubgen
        options = parse_options(["-v","-p",module_name,"--include-docstrings","--no-analysis", "--ignore-errors"])
        generate_stubs(options)

    fix_stubs_in_directory("out")

    module_path = os.path.join(*module_name.split("."))
    module_out_dir = os.path.join("out", module_path)
    target_dir = os.path.join(work_dir, module_path)


    if os.path.isdir(module_out_dir):
        # a package: out/Sofa/...
        shutil.copytree(module_out_dir, target_dir, dirs_exist_ok=True)
        print(f"Resync terminated for copying '{module_name}' to '{target_dir}'")
    elif os.path.isfile(module_out_dir + ".pyi"):
        # a single module: out/Sofa/Component/SolidMechanics/Spring.pyi
        os.makedirs(os.path.dirname(target_dir), exist_ok=True)
        shutil.copyfile(module_out_dir + ".pyi", target_dir + ".pyi")
        print(f"Resync terminated for copying '{module_name}' to '{target_dir}.pyi'")

    shutil.rmtree("out", ignore_errors=True)

#Generate stubs for components using the factory
def generate_component_stubs(work_dir,target_name, usePybind11_stubgen = False):
    print(f"Generating stubgen for all components in Sofa.Components using custom sofaStubgen.py")

    # SOFA modules having python bindings (eg. Sofa.Component.SolidMechanics.Spring) are compiled modules: their
    # stubs are a .pyi file next to them, made of the stubs of the bindings completed by the stubs of the
    # components having no binding.
    bound_modules = find_bound_modules(target_name)
    for module_name in bound_modules:
        stale_stub_package = os.path.join(work_dir, *module_name.split("."))
        if os.path.isdir(stale_stub_package):
            print(f"Warning: the directory '{stale_stub_package}' shadows the python binding '{module_name}'. "
                  f"It has probably been generated by a previous version of this script and should be removed.")
        generate_module_stubs(module_name, work_dir, usePybind11_stubgen)

    components = load_component_list(target_name)
    create_sofa_stubs(components, "out/", bound_modules)

    for module_name in bound_modules:
        module_stub = os.path.join(*module_name.split(".")) + ".pyi"
        out_stub = os.path.join("out", module_stub)
        target_stub = os.path.join(work_dir, module_stub)
        if os.path.isfile(out_stub) and os.path.isfile(target_stub):
            with open(target_stub, "a") as target_file, open(out_stub) as out_file:
                target_file.write("\n" + out_file.read())
            os.remove(out_stub)
            print(f"Merged the stubs of the components without binding into '{target_stub}'")


    sofa_out_dir = os.path.join("out",*target_name.split('.'))
    target_dir = os.path.join(work_dir, *target_name.split('.'))

    if os.path.isdir(sofa_out_dir):
        #For now on we don't want no overriting of file, we thus use the parameter 'ignore=noOverrideFilter(sofa_out_dir,target_dir)' to skip files already existing in dest folder because it might brake the lib.
        # When file merging is supported by create_sofa_stubs this will be removed because then, we will want to override the dest files because we've already did the merge
        shutil.copytree(sofa_out_dir, target_dir, dirs_exist_ok=True, ignore=noOverrideFilter(sofa_out_dir,target_dir))
        print("Resync terminated.")

    shutil.rmtree("out", ignore_errors=True)
