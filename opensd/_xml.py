def clean_indentation(element, level=0, spaces_per_level=2, trailing_indent=True):
    """Set indentation of XML element and its sub-elements.
    Copied and pasted from https://effbot.org/zone/element-lib.htm#prettyprint.
    It walks your tree and adds spaces and newlines so the tree is
    printed in a nice way.

    Parameters
    ----------
    level : int
        Indentation level for the element passed in (default 0)
    spaces_per_level : int
        Number of spaces per indentation level (default 2)
    trailing_indent : bool
        Whether or not to add indentation after closing the element

    """
    i = "\n" + level*spaces_per_level*" "

    # ensure there's always some tail for the element passed in
    if not element.tail:
        element.tail = ""

    if len(element):
        if not element.text or not element.text.strip():
            element.text = i + spaces_per_level*" "
        if trailing_indent and (not element.tail or not element.tail.strip()):
            element.tail = i
        for sub_element in element:
            # `trailing_indent` is intentionally not forwarded to the recursive
            # call. Any child element of the topmost element should add
            # indentation at the end to ensure its parent's indentation is
            # correct.
            clean_indentation(sub_element, level+1, spaces_per_level)
        if not sub_element.tail or not sub_element.tail.strip():
            sub_element.tail = i
    else:
        if trailing_indent and level and (not element.tail or not element.tail.strip()):
            element.tail = i


def reorder_attributes(root):
    """Sort attributes in XML to preserve pre-Python 3.8 behavior

    Parameters
    ----------
    root : lxml.etree._Element
        Root element

    """
    for el in root.iter():
        attrib = el.attrib
        if len(attrib) > 1:
            # adjust attribute order, e.g. by sorting
            attribs = sorted(attrib.items())
            attrib.clear()
            attrib.update(attribs)
