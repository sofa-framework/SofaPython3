# STLIB

## Terminology

| Term           | Description                                                    |
| -------------- | -------------------------------------------------------------- |
| Component*     | Element of the scene hierarchy implementing a given behavior |
| ~~Object~~     | A deprecated synonym of a Component |
| Node*          | Element of the scene hierarchy holding other Nodes (often refered as childs) or Components       |
| Data*          | Attribute of a Component or a Node                                  |
| Prefab         | A Node assembling of Components and Nodes (a "fragment" of a scene)   |
| Geometry       | A Prefab that describes shapes with their topologies (i.e a shapes with their space descritization and their associated connectivity)  |
| Entity         | A physical Prefab that represents real-world properties and behaviors used in a simulation. An entity should always have a geometry but includes neither a linear solver nor an integration scheme.|
| Parameters     | Every Prefab has a set of parameters. These parameters can contain data, links, callable or being composed of other parameters. Some of them can be optional. ~~Must inherit from `stlib.core.baseParameter.BaseParameter` and have a `@dataclasses.dataclass` decorator~~. Must have a `@stlib.parameters` decorator. |

\*Defined in SOFA documentation [here](https://www.sofa-framework.org/doc/using-sofa/terminology).


## Concepts & Structure

This library is structured to provide a set of _Prefabs_ that can be used to build complex simulations in SOFA. 
Prefabs are reusable fragments of a scene that can be specified through Parameters.
We introduce two different concepts, Prefab and Parameters:
- Prefabs defining the logic of instantiation
- Parameters providing the information (e.g data, links, callable) needed by a Prefab for its own instantiation

We introduce two types of Prefabs:
- __Geometry__: A prefab that describes shapes with their topologies (i.e a shape with its space discretization and its associated connectivity).
- __Entity__: A physical prefab that represents real-world properties and behaviors used in a simulation. An entity should always have a geometry but includes neither a linear solver nor an integration scheme.


## Usage

STLIB has been designed to suit the following levels of use:

- __Beginners__: 
    - Create simple simulations using predefined Prefabs.
    - Use the provided Prefabs and Parameters without needing to understand the underlying implementation.
- __Intermediate users__: 
    - Create more complex simulations by combining existing Prefabs.
    - Redefine Parameters for their own usage.
- __Advanced users__:    
    - Create their own Prefabs from scratch or by extending the provided ones.
    - Enrich the library with new Prefabs and Parameters.

## Available Parameters

Each Prefab comes with a set of Parameters, these have been selected when the following criteria are met:
- Data which corresponds to an intraseque property of the Prefab, required for its instantiation
- Data which does not have a default value
- Data which cannot be inferred
- Data which are commun to all possible usage of the Prefab 
