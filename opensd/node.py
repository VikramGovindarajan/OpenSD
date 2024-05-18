
class Node(object):

    def __init__(self,identifier,circuit,volume,heat_input,elevation,tpres_old,ttemp_old,tenth_old,geom):
        """!@param identifier name specified by user (user input)
            @param heat_input heat input to the node (user input)
            @param esource energy source calculated for nodes with fixed 'T' or 'H' (output)
            @param msource mass source calculated for nodes with fixed 'P' (output) or mass source specified for nodes with fixed 'msource' (input)
        """
        ## name specified by user (user input)
        self.identifier = identifier
        self.circuit = circuit
        self.node_ind = len(self.circuit.nodes)

        if tpres_old is not None: self.tpres_old = tpres_old
        if ttemp_old is not None: self.ttemp_old = ttemp_old
        if tenth_old is not None: self.tenth_old = tenth_old
        
        self.ifaces=[]
        self.ofaces=[]
        
        self.heat_input = heat_input
        self.elevation = elevation
        
        self.fixed_var = []
        ## energy source calculated for nodes with fixed 'T' or 'H' (output)
        self.esource = 0. #for printing initialization
        self.mflow_in = 1.E-4
        ## mass source calculated for nodes with fixed 'P' (output) or \n mass source specified for nodes with fixed 'msource' (input)
        self.msource = 0.
        self.flowreg = "Homogeneous"
        self.hresidue = 0.
        self.mresidue = 0.
        self.volume = volume
        self.pc_flag = False
