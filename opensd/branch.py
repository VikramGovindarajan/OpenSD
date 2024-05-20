
class Branch(object):
    
    def __init__(self,start_node):
        self.nodes = []
        self.faces = []
        self.start_node = start_node
        self.nodes.append(start_node)
        self.circuit = start_node.circuit
        self.circuit.branches.append(self)
        self.branchno = len(self.circuit.branches)
        self.isolated = False
