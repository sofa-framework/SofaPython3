# coding: utf8

import Sofa
import Sofa.Helper
import unittest

class MsgHandler(Sofa.Helper.MessageHandler):
    def __init__(self):
        Sofa.Helper.MessageHandler.__init__(self)

    def process(self, msg):
        self.type = msg["type"]
        self.sender = msg["sender"]
        self.component = msg["component"]
        self.message = msg["message"]


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
        with MsgHandler() as handler:
            Sofa.Helper.msg_info("plop")
            self.assertEqual(handler.type, "Info")
            self.assertEqual(handler.sender, "PythonScript")
            self.assertEqual(handler.component, None)
            self.assertEqual(handler.message, "plop")

            Sofa.Helper.msg_warning("coucou")
            self.assertEqual(handler.type, "Warning")
            self.assertEqual(handler.sender, "PythonScript")
            self.assertEqual(handler.component, None)
            self.assertEqual(handler.message, "coucou")


