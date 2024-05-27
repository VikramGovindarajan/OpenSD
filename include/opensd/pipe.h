//! \file pipe.h

#ifndef OPENSD_PIPE_H
#define OPENSD_PIPE_H

#include "pugixml.hpp"

#include "opensd/node.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
//! \class Pipe
//==============================================================================

class Pipe {
public:
  std::string identifier; //!< User-defined identifier
  std::string dnode_str;
  std::string unode_str;
  double diameter; //!< Pipe diameter in [m]
  double length; //!< Pipe length in [m]
  explicit Pipe(pugi::xml_node pipe_node);
  Pipe() = default;
  // virtual ~Pipe() = default;
  int ncell;
  Node* unode;
  Node* dnode;

protected:

};


//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_PIPE_H
