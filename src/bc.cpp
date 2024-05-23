#include "opensd/bc.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

namespace model {
vector<unique_ptr<BC>> bcs;
} // namespace model

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

void read_bcs(pugi::xml_node node)
{

  for (pugi::xml_node bc_node : node.children("bc")) {
    model::bcs.push_back(make_unique<BC>(bc_node));
  }

}

} // namespace opensd
