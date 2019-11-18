import Sofa.Core
from Sofa.Helper import msg_info


class EmptyDataEngine(Sofa.Core.DataEngine):
    """ custom %EmptyDataEngine% component for SOFA """

    def __init__(self, *args, **kwargs):
        Sofa.Core.DataEngine.__init__(self, *args, **kwargs)
        pass

    def init():
        pass

    def update():
        """
        called anytime an output is accessed while the component
        is dirty (input has changed)
        """
        msg_info('Not implemented yet')
        pass
    
