import pygame
import time
from OpenGL.GL import *
from OpenGL.GLU import *

import threading
import Sofa, SofaRuntime
root = None

def startUI(fct):
    pygame.display.init()
    display = (800,600)
    pygame.display.set_mode(display, pygame.DOUBLEBUF|pygame.OPENGL)
                
    time = 0.0
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        #glRotatef(1, 3, 1, 1)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glEnable(GL_LIGHTING)
        glEnable(GL_DEPTH_TEST)
        if fct != None:
                fct(display, time)
        time+=0.01
        
        pygame.display.flip()

def show(scene, camera):
        def draw(display, t):
                print("DRAW "+str(time)+ " camera> " + camera.getPathName()+" pos " + str(camera.position.array()) )
                Sofa.Simulation.glewInit()
                
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();    
                gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)
                
                #cameraPM = camera.getProjectionMatrix()                     
                #lMultMatrixd(cameraPM);
                
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
                cameraMVM = camera.getOpenGLModelViewMatrix()                     
                glMultMatrixd(cameraMVM) 
                               
                glPointSize(10.0)
                glColor4d(1.0,1.0,1.0,1.0)
                glBegin(GL_POINTS)
                glVertex3d(0.0,0.0,0.0)
                glVertex3d(1.0,0.0,0.0)
                glEnd()

                Sofa.Simulation.draw(scene)
                
        p = threading.Thread(target=startUI, args=(draw,))
        p.start()
        #def up(self):
        #        print("UPDATE")
        #        time.sleep(0.0000001)
        
        #setattr(p1,"update", up)
        return p

