#include "opensd/circuit.h"

#include <iostream>

#include "opensd/node.h"
#include "opensd/error.h"
#include "opensd/vector.h"
#include "opensd/memory.h"
#include "opensd/xml_interface.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

namespace model {
vector<unique_ptr<Circuit>> circuits;
} // namespace model

//==============================================================================
// Circuit implementation
//==============================================================================

Circuit::Circuit(pugi::xml_node cir_node)
{
  if (check_for_node(cir_node, "identifier")) {
    identifier_ = get_node_value(cir_node, "identifier");
  } else {
    fatal_error("Must specify identifier of circuit in geometry XML file.");
  }
        
  // Read the circuit fluid
  std::string flname_str {get_node_value(cir_node, "flname")};
  
  // Read the circuit nodes
  read_flnodes(cir_node);

}

void read_circuits(pugi::xml_node node)
{
  // Count the number of nodes
  int n_circuits = 0;
  for (pugi::xml_node cir_node : node.children("circuit")) {
    n_circuits++;
    model::circuits.push_back(make_unique<Circuit>(cir_node));
  }
  std::cout << "no. of circuits = " << n_circuits << std::endl;


}

} // namespace opensd
