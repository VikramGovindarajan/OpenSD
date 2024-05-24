//! \file node.h

#ifndef OPENSD_NODE_H
#define OPENSD_NODE_H

#include "pugixml.hpp"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

class Node;

//==============================================================================
//! \class Node
//==============================================================================

class Node {
public:
  std::string identifier; //!< User-defined identifier
  explicit Node(pugi::xml_node flnode_node);
  Node() = default;
  double mresidue;
  double eqn_cont();
};

//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_NODE_H
