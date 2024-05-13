from enum import Enum
import numpy as np
from pathlib import Path
import lxml.etree as ET
from opensd.checkvalue import PathLike
from ._xml import clean_indentation, reorder_attributes

class RunMode(Enum):
    STEADY = 'steady'
    DESIGN = 'design'
    SENSITIVITY = 'sensitivity'
    OPTIMIZE = 'optimize'
    TRANSIENT_RESTART = 'transient restart'

class Settings:
    """Settings used for an OpenSD simulation.

    Parameters
    ----------
    **kwargs : dict, optional
        Any keyword arguments are used to set attributes on the instance.

    Attributes
    ----------
    tim_slot : np.array()
        Time slot.
    run_mode : {'steady', 'design', 'sensitivity', 'optimize', 'transient restart'}
        The type of calculation to perform (default is 'steady')
    """

    def __init__(self, **kwargs):
        
        self._run_mode = RunMode.STEADY
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


    def to_xml_element(self):
        """Create a 'settings' element to be written to an XML file.

        """
        # Reset xml element tree
        element = ET.Element("settings")
        self._create_run_mode_subelement(element)
        self._create_tim_slot_subelement(element)

        # Clean the indentation in the file to be user-readable
        clean_indentation(element)
        reorder_attributes(element)
        
        return element

    def _create_run_mode_subelement(self, root):
        elem = ET.SubElement(root, "run_mode")
        elem.text = self._run_mode.value

    def _create_tim_slot_subelement(self, root):
        elem = ET.SubElement(root, "tim_slot")
        elem.text = np.array_str(self._tim_slot)
