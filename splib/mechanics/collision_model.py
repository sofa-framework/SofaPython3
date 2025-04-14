from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import DEFAULT_VALUE
from splib.core.enum_types import ElementType



@ReusableMethod
def addCollisionModels(node,points=False, edges=False,triangles=False, spheres=False,tetrahedron=False,selfCollision=DEFAULT_VALUE, proximity=DEFAULT_VALUE, group=DEFAULT_VALUE, contactStiffness=DEFAULT_VALUE, contactFriction=DEFAULT_VALUE,spheresRadius=DEFAULT_VALUE,**kwargs):
    if(points):
        node.addObject("PointCollisionModel",name="PointCollision", selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, **kwargs)
    if(edges):
        node.addObject("LineCollisionModel",name="EdgeCollision", selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group,**kwargs)
    if(triangles):
        node.addObject("TriangleCollisionModel",name="TriangleCollision", selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group,**kwargs)
    if(spheres):
        node.addObject("SphereCollisionModel",name="SphereCollision", selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group, radius=spheresRadius, **kwargs)
    if(tetrahedron):
        node.addObject("TetrahedronCollisionModel",name="TetraCollision", selfCollision=selfCollision, proximity=proximity, contactStiffness=contactStiffness, contactFriction=contactFriction, group=group,**kwargs)
