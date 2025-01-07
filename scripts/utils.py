import sys
import os
import shutil
from sofaStubgen import load_component_list, create_sofa_stubs
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

    out_dir, sub_dir = to_output_and_subdir(
        output_dir=args.output_dir,
        module_name=args.module_name,
        root_suffix=args.root_suffix,
    )

    run(
        parser,
        printer,
        args.module_name,
        out_dir,
        sub_dir=sub_dir,
        dry_run=args.dry_run,
        writer=Writer(stub_ext=args.stub_extension),
    )

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

    module_out_dir = os.path.join("out", module_name)
    target_dir = os.path.join(work_dir, module_name)


    if os.path.isdir(module_out_dir):
        shutil.copytree(module_out_dir, target_dir, dirs_exist_ok=True)
        print(f"Resync terminated for copying '{module_name}' to '{target_dir}'")

    shutil.rmtree("out", ignore_errors=True)

#Generate stubs for components using the factory
def generate_component_stubs(work_dir,target_name):
    print(f"Generating stubgen for all components in Sofa.Components using custom sofaStubgen.py")

    components = load_component_list(target_name)
    create_sofa_stubs(components, "out/")


    sofa_out_dir = os.path.join("out", "Sofa")
    target_dir = os.path.join(work_dir, "Sofa")


    if os.path.isdir(sofa_out_dir):
        shutil.copytree(sofa_out_dir, target_dir, dirs_exist_ok=True)
        print("Resync terminated.")

    shutil.rmtree("out", ignore_errors=True)
