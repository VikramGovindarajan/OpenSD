
class BC(object):
    def __init__(self,identifier,bnode,bvar,bval,msrc_cond,trans,enabled):
        self.identifier=identifier
        self.var=bvar            
        self.bval=bval
        self.trans=trans
        self.enabled = enabled
        self.node = bnode
        bnode.fixed_var.append(bvar)
        self.msrc_cond = msrc_cond
