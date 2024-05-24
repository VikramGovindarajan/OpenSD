#include "opensd/bc.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// BC implementation
//==============================================================================

BC::BC(pugi::xml_node bc_node)
{
  if (check_for_node(bc_node, "identifier")) {
    identifier_ = get_node_value(bc_node, "identifier");
  } else {
    fatal_error("Must specify identifier of BC in geometry XML file.");
  }

  val_  = stod(get_node_value(bc_node, "val"));
  var_  = get_node_value(bc_node, "var");
  node_ = get_node_value(bc_node, "node");

}


} // namespace opensd
