import numpy as np
import lxml.etree as ET
from opensd.node import Node,Reservoir,TPTank
from opensd.pipe import Pipe
from opensd.bc import BC
from opensd.project import get_comp

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
    _registry = []
    
    def __init__(self, **kwargs):
        self._registry.append(self)
        self.identifier = ""
        self.solveSS = True
        self.nodes = []
        self.pipes = []
        self.BCs =[]
        self.pumps=[]
        self.orifices=[]
        self.Pbound_ind = []
        self.eps_m = self.mean_flow = self.eps_h = self.eps_p = 0.
        self.faces = []
        self.branches = []
        
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

    def add_tptank(self,identifier,volume=np.array(0.),heat_input=np.array(0.),elevation=np.array(0.),tpres_old=None,ttemp_old=None,tenth_old=None,geom=None):
        if not geom:
            sys.exit("specify geometry for tptank")
        node = TPTank(identifier,self,volume,heat_input,elevation,tpres_old,ttemp_old,tenth_old,geom)
        self.nodes.append(node)
        return node

    def add_reservoir(self,identifier,volume=np.array(0.),heat_input=np.array(0.),elevation=np.array(0.),tpres_old=None,ttemp_old=None,tenth_old=None,geom=None):
        node = Reservoir(identifier,self,volume,heat_input,elevation,tpres_old,ttemp_old,tenth_old,geom)
        self.nodes.append(node)
        return node


    def add_pipe(self,identifier,diameter,length,unode,dnode,fricopt,roughness,ncell,heat_input=0.,cfarea=None,npar=1,qcrit=None,Kforward=0.,flowreg="Homogeneous",ufrac=None,dfrac=None):
        unode = get_comp(unode)
        dnode = get_comp(dnode)
        if isinstance(unode,Reservoir) and ufrac is None:
            sys.exit("pipe " + identifier + " connected to tptank. please specify upstream connection fraction. stopping")
        if isinstance(dnode,Reservoir) and dfrac is None:
            sys.exit("pipe " + identifier + " connected to tptank. please specify downstream connection fraction. stopping")
        if flowreg == "Homogeneous":
            pipe = Pipe(identifier,self,diameter,length,unode,ufrac,dnode,dfrac,fricopt,roughness,ncell,heat_input,cfarea,npar,qcrit,Kforward,flowreg)
        elif flowreg == "Slug":
            from PINET import slug_pipe
            pipe = slug_pipe.SlugPipe(identifier,self,diameter,length,unode,dnode,fricopt,roughness,ncell,heat_input,cfarea,npar,qcrit,Kforward,flowreg)
        else:
            print ("flowreg not found.stopping",flowreg)
            sys.exit()
        self.pipes.append(pipe)
        return pipe

    def add_pump(self,identifier,unode,dnode,data,Nop,flowreg="Homogeneous",opt="curves",ufrac=None,dfrac=None):
        unode = get_comp(unode)
        dnode = get_comp(dnode)
        if isinstance(unode,Reservoir) and ufrac is None:
            sys.exit("pump " + identifier + " connected to tptank. please specify upstream connection fraction. stopping")
        if isinstance(dnode,Reservoir) and dfrac is None:
            sys.exit("pump " + identifier + " connected to tptank. please specify downstream connection fraction. stopping")
        from PINET import turbo_components as turbo_comp
        if opt=="curves":
            pump = turbo_comp.VSPump(identifier,unode,ufrac,dnode,dfrac,data,Nop,flowreg)
        elif opt=="homo":
            pump = turbo_comp.HPump(identifier,unode,ufrac,dnode,dfrac,data,Nop,flowreg)
        else:
            sys.exit ("check pump modeling option. stopping")
        self.pumps.append(pump)
        return pump

    def add_orifice(self,identifier,unode,dnode,diameter,Cd,opening=None,ufrac=None,dfrac=None):
        unode = get_comp(unode)
        dnode = get_comp(dnode)
        from PINET import flow_orifices as or_comp
        orifice = or_comp.Orifice(identifier,self,unode,ufrac,dnode,dfrac,diameter,Cd,opening)
        self.orifices.append(orifice)
        return orifice
        
    def add_BC(self,identifier,bnode,bvar,bval,msrc_cond=None,trans=True,enabled=True):
        bnode = get_comp(bnode)
        bc = BC(identifier,bnode,bvar,bval,msrc_cond,trans,enabled)
        self.BCs.append(bc)
        return bc

    def to_xml_element(self):
        """Create a 'circuit' element to be written to an XML file.

        """
        # Reset xml element tree
        element = ET.Element("circuit")
        element.set("identifier", str(self.identifier))
        
        if self.solveSS:
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
