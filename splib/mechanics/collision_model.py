from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import DEFAULT_VALUE
from splib.core.enum_types import CollisionPrimitive


@ReusableMethod
def addCollisionModels(node, primitive : CollisionPrimitive, 
                       topology=DEFAULT_VALUE, 
                       selfCollision=DEFAULT_VALUE, 
                       proximity=DEFAULT_VALUE, 
                       group=DEFAULT_VALUE, 
                       contactStiffness=DEFAULT_VALUE, 
                       contactFriction=DEFAULT_VALUE, 
                       spheresRadius=DEFAULT_VALUE, 
                       **kwargs):
    match primitive:
        case CollisionPrimitive.POINTS:
            node.addObject("PointCollisionModel", name="PointCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, **kwargs)
            return
        case CollisionPrimitive.LINES:            
            node.addObject("PointCollisionModel", name="PointCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, **kwargs)
            node.addObject("LineCollisionModel", name="EdgeCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, **kwargs)
            return
        case CollisionPrimitive.TRIANGLES:
            node.addObject("PointCollisionModel", name="PointCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, **kwargs)
            node.addObject("LineCollisionModel", name="EdgeCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, **kwargs)
            node.addObject("TriangleCollisionModel", name="TriangleCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group,**kwargs)
            return
        case CollisionPrimitive.SPHERES:
            node.addObject("SphereCollisionModel", name="SphereCollision", topology=topology, selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, radius=spheresRadius, **kwargs)
            return
        case _:
            return
        
        #Cube and tetra are missing. 
            
    