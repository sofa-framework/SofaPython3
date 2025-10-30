from stlib.core.baseParameters import BaseParameters, dataclasses

@dataclasses.dataclass
class ControlJointValue:
    Kp: float = 0.0
    Kd: float = 0.0
    Ki: float = 0.0
    i_clamp: float = 0.0
    pos_desired: float = 0.0
    vel_desired: float = 0.0
 
@dataclasses.dataclass
class SpotRobotParameters(BaseParameters): 
    name : str = "SpotRobot"

    spot_ctrl_joint_infos = {
        "fl.hy": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, 0.91, 0.0),
        "fl.kn": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, -1.52, 0.0),
        "fr.hy": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, 0.91, 0.0),
        "fr.kn": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, -1.52, 0.0),
        "hl.hy": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, 0.91, 0.0),
        "hl.kn": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, -1.52, 0.0),
        "hr.hy": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, 0.91, 0.0),
        "hr.kn": ControlJointValue(  10000.0, 0.01, 1.0, 14.0, -1.52, 0.0),
    }

    def toDict(self):
        return dataclasses.asdict(self)
