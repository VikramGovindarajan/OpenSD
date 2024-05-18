from pathlib import Path
import lxml.etree as ET
from ._xml import clean_indentation, reorder_attributes
from opensd.checkvalue import PathLike, CheckedList
from opensd.circuit import Circuit

class Model(CheckedList):
    """Model representing a collection of circuits and heat slabs.

    This class corresponds directly to the model.xml input file. It can be
    thought of as a normal Python list where each member is a :class:`Circuit`
    or :class:`HeatSlab`.
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
        Items (circuits or heatslabs) to add to the collection

    """

    def __init__(self, items=None):
        super().__init__(Circuit, 'circuits collection')

        if items is not None:
            self += items

    def export_to_xml(self, path: PathLike = 'model.xml'):
        """Export model to an XML file.

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

        # Write the <circuit> or <hslab> elements.
        for item in sorted(self, key=lambda x: x.identifier):
            element = item.to_xml_element()
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
