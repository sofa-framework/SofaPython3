

class exportScene():
    def __init__(self,name="rootNode"):
        self.name = name

    def addObject(self,type:str,**kwargs):
        suffix = ""
        for i in kwargs:
            suffix += "," + str(i) + "=\"" + str(kwargs[i]) + "\""
        print(self.name+".addObject(\"" + type + "\"" +  suffix + ")")

    def addChild(self,name:str):
        print(name + '=' + self.name+".addChild(\"" + name + "\")")
        setattr(self,name,exportScene(name))
        return getattr(self,name)

    def __setattr__(self, key, value):
        if(not(key == "name")):
            print(self.__dict__["name"] + "." + key + " = " + str(value))
            self.__dict__[key] = value
        else:
            self.__dict__[key] = value
