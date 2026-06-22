import Sofa
import Sofa.Gui
import http
from threading import Thread

import json

from functools import cached_property
from http.cookies import SimpleCookie
from http.server import BaseHTTPRequestHandler
from http.server import HTTPServer
from urllib.parse import parse_qsl, urlparse

def dump_factory():
    d = {}
    
    for component in Sofa.Core.ObjectFactory.components:
        try: 
            node = Sofa.Core.Node("tmp")
            #obj = node.addObject(component.className)    
            d[component.className] = {
                "target" : [name for name in component.targets],
                "description" : component.description
            }
        except:
            pass
    return d

def scene_to_dict(node : Sofa.Core.Node) -> dict :
    d = {}
    for child in node.children:
        d += scene_to_dict(child)
    for object in node.objects:
        d[str(object.linkpath)] = { 
                "name": object.name.value,
                "type": object.getClassName()
        }

    return d

class WebRequestHandler(BaseHTTPRequestHandler):
    @cached_property
    def url(self):
        return urlparse(self.path)

    @cached_property
    def query_data(self):
        return dict(parse_qsl(self.url.query))

    @cached_property
    def post_data(self):
        content_length = int(self.headers.get("Content-Length", 0))
        return self.rfile.read(content_length)

    @cached_property
    def form_data(self):
        return dict(parse_qsl(self.post_data.decode("utf-8")))

    @cached_property
    def cookies(self):
        return SimpleCookie(self.headers.get("Cookie"))

    def do_GET(self):
        d = ""
        if self.url.path == "/load":
            print("Can you reload the scene for ", self.url.params)
            Sofa.Gui.GUIManager.SetScene(Sofa.Core.Node("YoLO"), filename="emptyController.py")
        if self.url.path == "/factory":
            print("Dump the factory in the browser ", self.url.params)
            d = json.dumps(dump_factory())
        else:
            print("Everything else for ", self.url)
            d = json.dumps(scene_to_dict(root_scene))
        
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.end_headers()
        self.wfile.write(d.encode("utf-8"))

def serve_http():
    server = HTTPServer(("0.0.0.0", 8000), WebRequestHandler)
    server.serve_forever()

root_scene = None

def createScene(root):
    global root_scene
    root_scene = root
    print("BEFORE RUN")
    t = Thread(target=serve_http)
    t.start()
    print("COUCOU")
    return root