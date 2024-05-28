//! \file geometry.h
//! Auxilary functions for geometry initialization and general data handling.

#ifndef OPENSD_GEOMETRY_AUX_H
#define OPENSD_GEOMETRY_AUX_H

#include "opensd/xml_interface.h"

namespace opensd {


//! Read geometry from XML file
void read_geometry_xml();

//! Read geometry from XML node
//! \param[in] root node of geometry XML element
void read_geometry_xml(pugi::xml_node root);

} // namespace opensd

#endif // OPENSD_GEOMETRY_AUX_H
