#include "opensd/node.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Node implementation
//==============================================================================

Node::Node(pugi::xml_node flnode_node)
{
  if (check_for_node(flnode_node, "identifier")) {
    identifier_ = get_node_value(flnode_node, "identifier");
  } else {
    fatal_error("Must specify identifier of flow node in geometry XML file.");
  }

}

} // namespace opensd
