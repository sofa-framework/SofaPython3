from stlib.entities import Entity
from compliancerobotics.robots.spot.__parameters__ import SpotRobotParameters
import numpy as np

class SpotRobot(Entity):
    """Boston dynamic spot robots"""
    @staticmethod
    def get_asset(name) -> str:
        import os
        return os.path.join(os.path.join(os.path.dirname(__file__), "assets"), name)

    @staticmethod
    def getParameters(**kwargs) -> SpotRobotParameters:
        return SpotRobotParameters(**kwargs)

    def __init__(self, parameter : SpotRobotParameters = None, **kwargs):
        if parameter is None:
            parameter = SpotRobotParameters(**kwargs)
        Entity.__init__(self, parameter)

        self.create(parameter)

    def create(self, parameter):
        # Robot node
        settings = self.addChild('settings')
        settings.addObject("RequiredPlugin", name="SoftRobots") 
        settings.addObject("RequiredPlugin", name="Sofa.RigidBodyDynamics") # Needed to use components [URDFModelLoader]
        settings.addObject('RequiredPlugin', name='Sofa.Component.Mapping.NonLinear') # Needed to use components [RigidMapping]  
        settings.addObject('RequiredPlugin', name='Sofa.Component.Mass') # Needed to use components [UniformMass]
        settings.addObject('RequiredPlugin', name='Sofa.Component.Topology.Container.Constant') # Needed to use components [MeshTopology]  
        settings.addObject('RequiredPlugin', name='Sofa.GL.Component.Rendering3D') # Needed to use components [OglModel]  

        self.addObject('URDFModelLoader', 
                        filename=SpotRobot.get_asset("model.urdf"), 
                        modelDirectory=SpotRobot.get_asset(""), 
                        useFreeFlyerRootJoint=False, 
                        printLog=False, 
                        addCollision=False, 
                        addJointsActuators=False)
        
        robot = self.getChild("Robot")
        robot.name = "model"
        mechanical = robot.Model.getMechanicalState()
        mechanical.showObject = False
        mechanical.showObjectScale = 0.01
        mechanical.drawMode = 0 

        # Direct problem
        names = robot.Joints.children
        positions = np.copy(robot.getMechanicalState().position.value)
        for i in range(len(positions)):
            jointName = names[i+1].name.value
            value = 0 if jointName not in parameter.spot_ctrl_joint_infos else parameter.spot_ctrl_joint_infos[jointName].pos_desired
            positions[i] = value
            joint = robot.addObject('JointConstraint', template='Vec1', name='joint' + jointName, index=i, 
                                valueType="angle", 
                                value=value
                                )
        