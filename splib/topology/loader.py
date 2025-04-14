from splib.core.node_wrapper import ReusableMethod

@ReusableMethod
def loadMesh(node,filename,**kwargs):
    splitedName = filename.split('.')
    if len(splitedName) == 1:
        print('[Error] : A file name without extension was provided.')
        return

    if splitedName[-1] in ['vtk', 'obj', 'stl', 'msh', 'sph']:
        if splitedName[-1] == "msh":
            return node.addObject("MeshGmshLoader", name="loader",filename=filename, **kwargs)
        elif splitedName[-1] == "sph":
            return node.addObject("SphereLoader", name="loader",filename=filename, **kwargs)
        else:
            return node.addObject("Mesh"+splitedName[-1].upper()+"Loader", name="loader",filename=filename, **kwargs)
    else:
        print('[Error] : File extension ' + splitedName[-1] + ' not recognised.')




