import lxml.etree as ET
from opensd.node import Node

class Circuit:
    """Flow circuit representing a collection of nodes, pipes, and other flow elements.

    Parameters
    ----------
    **kwargs : dict, optional
        Any keyword arguments are used to set attributes on the instance.

    Attributes
    ----------
    identifier : str
        Circuit identifier
    solveSS : bool
        Whether to solve SS (default is 'True')
    flname : str
        Fluid identifier (as per CoolProp Nomenclature)
    flap_tp : bool
        Whether to solve two-phase (default is 'False')
        
    """

    def __init__(self, **kwargs):
        
        self._identifier = ""
        self._solveSS = True
        self.nodes = []

        for key, value in kwargs.items():
            setattr(self, key, value)

    def assign_fluid(self,flname,flag_tp=False):
        self._flname = flname
        self._flag_tp = flag_tp
        
        # if fllib=="CoolProp":
            # self.flstate=CoolProp.AbstractState("BICUBIC&HEOS",self.flname)
        # elif fllib=="thiravam":
            # self.flstate=thiravam.state(self.flname)
        # elif fllib=="User":
            # sys.path.insert(0,os.getcwd() + "/")
            # mod = __import__(flname)
            # clas = getattr(mod,'fluid')
            # self.flstate=clas()
        # else:
            # print ("fluid library unknown. stopping")
            # sys.exit()

    def add_node(self,identifier,volume=0.,heat_input=0.,elevation=0.,tpres_old=None,ttemp_old=None,tenth_old=None,geom=None):
        node = Node(identifier,self,volume,heat_input,elevation,tpres_old,ttemp_old,tenth_old,geom)
        self.nodes.append(node)
        return node

    def to_xml_element(self):
        """Create a 'circuit' element to be written to an XML file.

        """
        # Reset xml element tree
        element = ET.Element("circuit")
        element.set("identifier", str(self._identifier))
        
        if self._solveSS:
            element.set("solveSS", "true")

        if self._flag_tp:
            element.set("flag_tp", "true")
        
        if self._flname is not None:
            subelement = ET.SubElement(element, "flname")
            subelement.text = self._flname
        else:
            raise ValueError(f'Fluid has not been assigned for circuit {self.identifier}!')

        if self.nodes:
            for node in self.nodes:
                subelement = ET.SubElement(element, "Node")
                subelement.set("identifier", node.identifier)
                if node.elevation != 0.:
                    subelement.set("elevation", str(node.elevation))

        return element
