import numpy as np
from pathlib import Path
import lxml.etree as ET
from ._xml import clean_indentation, reorder_attributes
from opensd.checkvalue import PathLike, CheckedList
    
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

        return element

class Model(CheckedList):
    """Collection of Model used for an OpenSD simulation.

    This class corresponds directly to the model.xml input file. It can be
    thought of as a normal Python list where each member is a :class:`Circuit`.
    It behaves like a list as the following example demonstrates:

    >>> circuit1 = openmc.Circuit()
    >>> circuit2 = openmc.Circuit()
    >>> heatslab1 = openmc.HeatSlab()
    >>> m = openmc.Model([circuit1])
    >>> m.append(circuit2)
    >>> m += [heatslab1]

    Parameters
    ----------
    model : Iterable of openmc.Circuit, openmc.HeatSlab
        Models to add to the collection

    """

    def __init__(self, circuits=None):
        super().__init__(Circuit, 'circuits collection')

        if circuits is not None:
            self += circuits

    def export_to_xml(self, path: PathLike = 'model.xml'):
        """Export circuit to an XML file.

        Parameters
        ----------
        path : str
            Path to file to write. Defaults to 'model.xml'.

        """

        # Check if path is a directory
        p = Path(path)
        if p.is_dir():
            p /= 'model.xml'

        with open(str(p), 'w', encoding='utf-8',
                  errors='xmlcharrefreplace') as fh:
            self._write_xml(fh)

    def _write_xml(self, file, header=True, level=0, spaces_per_level=2,
                   trailing_indent=True):
        """Writes XML content of the model to an open file handle.

        Parameters
        ----------
        file : IOTextWrapper
            Open file handle to write content into.
        header : bool
            Whether or not to write the XML header
        level : int
            Indentation level of model element
        spaces_per_level : int
            Number of spaces per indentation
        trailing_indentation : bool
            Whether or not to write a trailing indentation for the model element

        """
        indentation = level*spaces_per_level*' '
        # Write the header and the opening tag for the root element.
        if header:
            file.write("<?xml version='1.0' encoding='utf-8'?>\n")
        file.write(indentation+'<model>\n')

        # Write the <circuit> elements.
        for circuit in sorted(self, key=lambda x: x.identifier):
            element = circuit.to_xml_element()
            clean_indentation(element, level=level+1)
            element.tail = element.tail.strip(' ')
            file.write((level+1)*spaces_per_level*' ')
            reorder_attributes(element)  # TODO: Remove when support is Python 3.8+
            file.write(ET.tostring(element, encoding="unicode"))

        # Write the closing tag for the root element.
        file.write(indentation+'</model>\n')

        # Write a trailing indentation for the next element
        # at this level if needed
        if trailing_indent:
            file.write(indentation)
