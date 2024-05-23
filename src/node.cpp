#include "opensd/node.h"

#include <iostream>

#include "pugixml.hpp"

namespace opensd {

void read_flnodes(pugi::xml_node node)
{
  // Count the number of nodes
  int n_flnodes = 0;
  for (pugi::xml_node flnode_node : node.children("node")) {
    n_flnodes++;
  }
  std::cout<<"no. of nodes = "<<n_flnodes<<"\n";
}

} // namespace opensd
