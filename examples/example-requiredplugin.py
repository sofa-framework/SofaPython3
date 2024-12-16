import Sofa
import SofaRuntime
import os
import re

class RequiredPlugin(Sofa.Prefab):
    def __init__(self, pluginName):
        Sofa.Prefab.__init__(self, **{"name" : "RequiredPlugins("+pluginName+")"})
        pattern = "lib"+pluginName+".*\.so$"
        for path in SofaRuntime.PluginRepository.getPaths():
            self.findAllMatchingFiles(path, pattern,pluginName)

    def fillGraphFor(self, path, fileName, shortName, pluginName):
        print("Fill Graph For ", fileName)
        e = shortName.split(".")
        c = self
        for level in e[:-1]:
            if len(level) != 0:
                print("Create a Level", level)
                if level not in c.children:
                    c = c.addChild(level)
                else:
                    c = c.children[level]

        try:
            level = e[-1]
            if level not in c.children and level != "":
                if SofaRuntime.importPlugin(os.path.join(path, fileName)):
                    c.addObject("RequiredPlugin", pluginName=fileName[3:-3],name=level)
        except Exception as e:
            pass

    def findAllMatchingFiles(self, path, pattern,pluginName):
        entries = []
        for name in os.listdir(path):
            if re.match(pattern, name):
                entries.append([path, name, name[len(pluginName)+4:-3], pluginName])
        for entry in sorted(entries, key=lambda x: x[2],reverse=True):
            self.fillGraphFor(*entry)

def createScene(root):
    root.addChild(RequiredPlugin("Sofa.Component.SolidMechanics"))
    root.addObject("MechanicalObject")
