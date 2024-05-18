
class Pipe(object):

    def __init__(self,identifier,circuit,diameter,length,unode,ufrac,dnode,dfrac,fricopt,roughness,ncell,heat_input,cfarea,npar,qcrit,Kforward,flowreg):
        self.identifier=identifier
        self.circuit = circuit
        self.flowreg = flowreg

        self.diameter=diameter
        self.length=length

        self.ncell=ncell
        self.unode=unode
        self.ufrac = ufrac
        self.dnode=dnode
        self.dfrac = dfrac
        
        self.npar = npar
        self.qcrit = qcrit
        self.Kforward_old = self.Kforward = Kforward

        delx = length/ncell
        delz = (dnode.elevation - unode.elevation)/ncell
        if delz - delx > 1.E-6:
            print ("length is less than the elevation difference. stopping",self.identifier)
            sys.exit()
        
        self.cfarea1 = cfarea
        if cfarea is None:
            cfarea=math.pi*diameter**2/4.
            
        cfarea = cfarea * npar
            
        self.heat_input = heat_input

        self.nodes = []
        for i in range(ncell-1):
            node=circuit.add_node(identifier+"_node"+str(i),volume=delx*cfarea,elevation=unode.elevation+(dnode.elevation-unode.elevation)*(i+1)/ncell)
            node.i = i
            self.nodes.append(node)
        
        self.faces = []
        for i in range(ncell):
            if i==0 and ncell==1:
                self.faces.append(PFace(i,self,unode,ufrac,dnode,dfrac,diameter,cfarea,delx,delz,fricopt,roughness))
            elif i==0:
                self.faces.append(PFace(i,self,unode,ufrac,self.nodes[0],None,diameter,cfarea,delx,delz,fricopt,roughness))
            elif i==ncell-1:
                self.faces.append(PFace(i,self,self.nodes[ncell-2],None,dnode,dfrac,diameter,cfarea,delx,delz,fricopt,roughness))
            else:
                self.faces.append(PFace(i,self,self.nodes[i-1],None,self.nodes[i],None,diameter,cfarea,delx,delz,fricopt,roughness))
            
        for i in range(ncell-1):
            self.nodes[i].ifaces.append(self.faces[i])
            self.nodes[i].ofaces.append(self.faces[i+1])

        unode.ofaces.append(self.faces[0])
        unode.volume = unode.volume + 0.5*delx*cfarea

        dnode.ifaces.append(self.faces[-1])
        dnode.volume = dnode.volume + 0.5*delx*cfarea
        
        self.mflow = 0.
        
    # def add_wall(self,thk,solname,sollib,restraint):
        # wall = Wall(thk,solname,sollib,restraint)
        # for face in self.faces:
            # face.wall = wall
    