# coding: utf8

import Sofa
import Sofa.Helper
import unittest

class MsgHandler(Sofa.Helper.MessageHandler):
    def __init__(self):
        Sofa.Helper.MessageHandler.__init__(self)

    def process(msg):
        print ("GOT MESSAGE!")
        print("type: ")
        print(msg.type)
        print("sender: ")
        print(msg.sender)
        print("component: ")
        print(msg.component.typeName())
        print(msg.component.getName())
        print("message:")
        print(msg.message)


class Test(unittest.TestCase):
    def test_messages(self):
        """Test that the message are correctly sended and does not generates exceptions"""
        for fname in ["msg_info", "msg_warning", "msg_deprecated"]:
            f = getattr(Sofa.Helper, fname)
            f("Simple message")
            f("Emitter", "Simple message")
            f("Simple message with attached source info", "sourcefile.py", 10)
            f(Sofa.Core.Node("node"), "Simple message to an object")
            f(Sofa.Core.Node("node"), "Simple message to an object with attached source info", "sourcefile.py", 10)



    def test_messageHandler(self):
        with MsgHandler():
            Sofa.Helper.msg_info("plop")
            Sofa.Helper.msg_error("pouet")
        Sofa.Helper.msg_warning("coucou")


