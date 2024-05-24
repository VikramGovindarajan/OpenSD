#include "opensd/pipe.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Pipe implementation
//==============================================================================

Pipe::Pipe(pugi::xml_node pipe_node)
{
  if (check_for_node(pipe_node, "identifier")) {
    identifier = get_node_value(pipe_node, "identifier");
  } else {
    fatal_error("Must specify identifier of pipe in geometry XML file.");
  }

  diameter = stod(get_node_value(pipe_node, "diameter"));
  length   = stod(get_node_value(pipe_node, "length"));
  //dnode_    = get_node_value(pipe_node, "dnode");
  //unode_    = get_node_value(pipe_node, "unode");

}

} // namespace opensd
