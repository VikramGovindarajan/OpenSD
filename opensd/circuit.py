import numpy as np
import lxml.etree as ET
import CoolProp
from opensd.node import Node,Reservoir,TPTank
from opensd.pipe import Pipe
from opensd.bc import BC
from opensd.project import get_comp
from opensd.settings import Settings
# from opensd.branch import Branch

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
        self.bcs =[]
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
        self.flstate=CoolProp.AbstractState("BICUBIC&HEOS",self._flname)
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
        self.bcs.append(bc)
        return bc

    # def get_reference_prop(self):
        # plist = []
        # tlist = []
        # hlist = []
        # for BC in self.bcs:
            # if BC.enabled:
                # node = BC.node
                # if BC.var == 'msource':
                    # BC.node.msource = BC.bval
                    # if BC.msrc_cond is not None:
                        # BC.node.tenth_msrc = BC.node.self.calc_enth(BC.msrc_cond[0],BC.msrc_cond[1],BC.msrc_cond[2]) #note: mass source conditions to be specified as static quantities e.g. static pressure and static enthalpy. Unlike boundary conditions, where total quantities are specified
                        # # hlist.append(BC.node.tenth_msrc)
                # elif BC.var == 'P':
                    # BC.node.tpres_old = BC.bval
                    # plist.append(BC.node.tpres_old)
                    # BC.node.circuit.Pbound_ind.append(BC.node.node_ind)
                # elif BC.var == 'T':
                    # BC.node.ttemp_old = BC.bval
                    # tlist.append(BC.node.ttemp_old)
                # elif BC.var == "H":
                    # BC.node.tenth_old = BC.bval
                    # if 'msource' in BC.node.fixed_var:
                        # BC.node.tenth_msrc = BC.node.tenth_old
                    # hlist.append(BC.node.tenth_old)
                # else:
                    # print ("improper boundary condition. stopping",BC.var)
                    # sys.exit()

        # self.Pbound_ind.sort()

        # for BC in self.bcs:
            # if BC.enabled and BC.var == 'P':
                # if 'T' in BC.node.fixed_var:
                    # BC.node.tenth_old = BC.node.self.calc_enth(CoolProp.PT_INPUTS,BC.node.tpres_old,BC.node.ttemp_old)
                    # hlist.append(BC.node.tenth_old)
                # elif 'H' in BC.node.fixed_var:
                    # BC.node.ttemp_old = BC.node.self.calc_temp(CoolProp.HmassP_INPUTS,BC.node.tenth_old,BC.node.tpres_old)
                    # tlist.append(BC.node.ttemp_old)

        # if len(plist) == 0:
            # print ("warning: ambient pressure assumed as reference pressure for the circuit")
            # pref = solver_settings.p_ambient
        # else:
            # pmax = max(plist)
            # pref = pmax #pmean = sum(plist)/len(plist) didn't converge for validation case 6
        # if len(tlist) == 0 and len(hlist) == 0:
            # print ("warning: ambient temperature assumed in the circuit")
            # tmean = Settings.T_ambient
            # tref = tmean
            # href = self.calc_enth(CoolProp.PT_INPUTS,pref,tref)
        # elif len(tlist) == 0 and len(hlist) != 0:
            # hmean = sum(hlist)/len(hlist)
            # href=hmean
            # tref = self.calc_temp(CoolProp.HmassP_INPUTS,href,pref)
        # elif len(hlist) == 0 and len(tlist) != 0:
            # tmean = sum(tlist)/len(tlist)
            # tref=tmean
            # href = self.calc_enth(CoolProp.PT_INPUTS,pref,tref)
        # else:
            # tmean = sum(tlist)/len(tlist)
            # tref=tmean
            # hmean = sum(hlist)/len(hlist)
            # href=hmean
        # return pref,tref,href

    # def create_branches(self,pref,tref,href):
        # for node in self.nodes:
            # node.blocking = False
            # if 'P' in node.fixed_var or 'msource' in node.fixed_var or len(node.ofaces) != 1 or len(node.ifaces) != 1:
                # node.blocking = True
        
        # for face in self.faces:
            # face.assigned = False
        
        # for face in self.faces:
            # if not face.assigned: 
                # branch = Branch(face.unode)
                # hnode = face.dnode
                # while not hnode.blocking:
                    # branch.nodes.append(hnode)
                    # branch.faces.append(face)
                    # face.branch = branch
                    # face.assigned = True
                    # face = hnode.ofaces[0]
                    # hnode = face.dnode
                # else:
                    # branch.nodes.append(hnode)
                    # branch.faces.append(face)
                    # face.assigned = True
                    # face.branch = branch

        # for branch in self.branches:
            # branch.hlist = []
            # branch.plist = []
            # for node in branch.nodes:
                # if hasattr(node,'tenth_old'):
                    # branch.hlist.append(node.tenth_old)
                # if hasattr(node,'tpres_old'):
                    # branch.plist.append(node.tpres_old)
            # if branch.hlist != []:
                # branch.href = max(branch.hlist)
            # if branch.plist != []:
                # branch.pref = max(branch.plist)
            
        # for node in self.nodes: #to assign enth for guess valued nodes
            # if hasattr(node,'ttemp_old') and 'T' not in node.fixed_var and 'H' not in node.fixed_var:
                # if 'P' in node.fixed_var or hasattr(node,'tpres_old'):
                    # node.tenth_old = self.calc_enth(CoolProp.PT_INPUTS,node.tpres_old,node.ttemp_old)
                # else:
                    # node.tenth_old = self.calc_enth(CoolProp.PT_INPUTS,pref,node.ttemp_old)
            # if hasattr(node,'tenth_old') and ('H' not in node.fixed_var and not('P' in node.fixed_var and 'T' in node.fixed_var)):
                # if 'P' in node.fixed_var or hasattr(node,'tpres_old'):
                    # node.circuit.flstate.update(CoolProp.HmassP_INPUTS,node.tenth_old,node.tpres_old)
                    # node.ttemp_old = node.circuit.flstate.T()
                # else:
                    # node.circuit.flstate.update(CoolProp.HmassP_INPUTS,node.tenth_old,pref)
                    # node.ttemp_old = node.circuit.flstate.T()

        # for branch in self.branches:
            # for node in branch.nodes:
                # if not hasattr(node,'tenth_old') and hasattr(branch,'href'):
                    # node.tenth_old = branch.href
                # if not hasattr(node,'tpres_old') and hasattr(branch,'pref'):
                    # node.tpres_old = branch.pref

        # for branch in self.branches:
            # if 'msource' in branch.nodes[0].fixed_var:
                # if branch.nodes[0].msource == 0.:
                    # branch.isolated = True

        # for node in self.nodes: #user nodes
            # if not hasattr(node,'tpres_old'):
                # if node.identifier.find("pipe") == -1:
                    # node.tpres_old = pref
        # for node in self.nodes: #internal nodes
            # if not hasattr(node,'tpres_old'):
                # if node.identifier.find("pipe") != -1:
                    # for pipe in self.pipes:
                        # for i,cell in enumerate(pipe.nodes):
                            # unode = pipe.faces[0].unode
                            # dnode = pipe.faces[-1].dnode
                            # pres = unode.tpres_old+(dnode.tpres_old-unode.tpres_old)*(i+1)/pipe.ncell
                            # cell.tpres_old = pres
            
            # if not hasattr(node,'ttemp_old'):
                # node.ttemp_old = tref
            # if not hasattr(node,'tenth_old'):
                # node.tenth_old = href
                
            # node.assign_staticvar()
            # # node.assign_prop(node.spres_old,node.senth_old)
            # # node.update_gues()
            # # print node.spres_gues

        # for face in self.faces:
            # face.assign_statevar()
            # # face.assign_prop(self.flag_tp,"Homogeneous")
            # # face.update_gues()

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
                subelement = ET.SubElement(element, "node")
                subelement.set("identifier", node.identifier)
                if node.elevation != 0.:
                    subelement.set("elevation", str(node.elevation))

        if self.pipes:
            for pipe in self.pipes:
                subelement = ET.SubElement(element, "pipe")
                subelement.set("identifier", pipe.identifier)
                subelement.set("diameter",   str(pipe.diameter))
                subelement.set("length",     str(pipe.length))
                subelement.set("unode",      str(pipe.unode.identifier))
                subelement.set("dnode",      str(pipe.dnode.identifier))
                
                # if node.elevation != 0.:
                    # subelement.set("elevation", str(node.elevation))

        if self.bcs:
            for bc in self.bcs:
                subelement = ET.SubElement(element, "bc")
                subelement.set("identifier", bc.identifier)
                subelement.set("node",       bc.node.identifier)
                subelement.set("var",        bc.var)
                subelement.set("bval",       str(bc.bval))
                
                # if node.elevation != 0.:
                    # subelement.set("elevation", str(node.elevation))

        return element

    # def calc_enth(self,input_pair,val1,val2):
        # if input_pair == 9:
            # self.flstate.update(input_pair,val1,val2)
            # return self.flstate.hmass()
        # else:
            # print ("input_pair not recognized. stopping",input_pair)
            # sys.exit()
