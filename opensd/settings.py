import numpy as np
from pathlib import Path
import lxml.etree as ET
from opensd.checkvalue import PathLike

class Settings:
    """Settings used for an OpenSD simulation.

    Parameters
    ----------
    **kwargs : dict, optional
        Any keyword arguments are used to set attributes on the instance.

    Attributes
    ----------
    delt : float
        Time step size.
    etime : float
        Simulation end time.
    """

    def __init__(self, **kwargs):
        
        self._tim_slot = np.array([0.])
        # if hasattr(scheduler,"tim_slot"):
            # tim_slot = scheduler.tim_slot
        # else:
            # tim_slot = np.arange(scheduler.delt,scheduler.etime+scheduler.delt,scheduler.delt)

        for key, value in kwargs.items():
            setattr(self, key, value)

    def export_to_xml(self, path: PathLike = 'settings.xml'):
        """Export simulation settings to an XML file.

        Parameters
        ----------
        path : str
            Path to file to write. Defaults to 'settings.xml'.

        """
        root_element = self.to_xml_element()

        # Check if path is a directory
        p = Path(path)
        if p.is_dir():
            p /= 'settings.xml'

        # Write the XML Tree to the settings.xml file
        tree = ET.ElementTree(root_element)
        tree.write(str(p), xml_declaration=True, encoding='utf-8')


    def to_xml_element(self, mesh_memo=None):
        """Create a 'settings' element to be written to an XML file.

        Parameters
        ----------
        mesh_memo : set of ints
            A set of mesh IDs to keep track of whether a mesh has already been written.
        """
        # Reset xml element tree
        element = ET.Element("settings")

        return element
