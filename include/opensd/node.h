//! \file node.h

#ifndef OPENSD_NODE_H
#define OPENSD_NODE_H

#include "pugixml.hpp"
#include "opensd/vector.h"
#include "opensd/memory.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

class Node;

namespace model {
extern vector<unique_ptr<Node>> nodes;
} // namespace model

//==============================================================================
//! \class Node
//==============================================================================

class Node {
public:
  std::string identifier_; //!< User-defined identifier
  explicit Node(pugi::xml_node flnode_node);

protected:

};

//==============================================================================
// Non-member functions
//==============================================================================

void read_flnodes(pugi::xml_node node);

} // namespace opensd

#endif // OPENSD_NODE_H
