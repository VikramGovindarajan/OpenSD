import lxml.etree as ET

class BC(object):
    def __init__(self,identifier,bnode,bvar,bval,msrc_cond,trans,enabled):
        self.identifier=identifier
        self.var=bvar            
        self.val=bval
        self.trans=trans
        self.enabled = enabled
        self.node = bnode
        bnode.fixed_var.append(bvar)
        self.msrc_cond = msrc_cond

    def to_xml_element(self,element):
        subelement = ET.SubElement(element, "bc")
        subelement.set("identifier", self.identifier)
        subelement.set("node",       self.node.identifier)
        subelement.set("var",        self.var)
        subelement.set("val",       str(self.val))
