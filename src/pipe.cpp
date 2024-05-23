#include "opensd/pipe.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

namespace model {
vector<unique_ptr<Pipe>> pipes;
} // namespace model

//==============================================================================
// Pipe implementation
//==============================================================================

Pipe::Pipe(pugi::xml_node pipe_node)
{
  if (check_for_node(pipe_node, "identifier")) {
    identifier_ = get_node_value(pipe_node, "identifier");
  } else {
    fatal_error("Must specify identifier of pipe in geometry XML file.");
  }

  diameter_ = stod(get_node_value(pipe_node, "diameter"));
  length_   = stod(get_node_value(pipe_node, "length"));
  dnode_    = get_node_value(pipe_node, "dnode");
  unode_    = get_node_value(pipe_node, "unode");

}

void read_pipes(pugi::xml_node node)
{

  for (pugi::xml_node pipe_node : node.children("pipe")) {
    model::pipes.push_back(make_unique<Pipe>(pipe_node));
  }

}

} // namespace opensd
