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
    verbosity : int
        Verbosity during simulation between 1 and 10. Verbosity levels are
        described in :ref:`verbosity`.
    """
    T_ambient = 300.

    def __init__(self, **kwargs):
        
        self._run_mode = RunMode.STEADY
        self._tim_slot = np.array([0.])
        self._verbosity = None

        self._no_main_iter = 2500

        self._conv_crit_flow=1.E-10
        self._no_flow_iter = 300

        self._p_ambient = 1.E5

        self._temp_solve = True
        self._conv_crit_temp_SS = 1.E-10
        self._conv_crit_temp_trans = self._conv_crit_temp_SS

        self._conv_crit_ht = 1.E-10

        self._ZERO = 1.E-8

        self._show_warn = False

        self._alpha_mom = np.array(0.6)
        self._alpha_heat = np.array(0.6)
        self._alpha_ener = np.array(0.6)

        
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
        self._create_verbosity_subelement(element)
        
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

    def _create_verbosity_subelement(self, root):
        if self._verbosity is not None:
            element = ET.SubElement(root, "verbosity")
            element.text = str(self._verbosity)
