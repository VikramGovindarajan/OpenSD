#include "opensd/node.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

namespace model {
vector<unique_ptr<Node>> nodes;
} // namespace model

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

void read_flnodes(pugi::xml_node node)
{
  // Count the number of nodes
  int n_flnodes = 0;
  for (pugi::xml_node flnode_node : node.children("node")) {
    n_flnodes++;
    model::nodes.push_back(make_unique<Node>(flnode_node));
  }
  std::cout<<"no. of nodes = "<<n_flnodes<<"\n";
}

} // namespace opensd
