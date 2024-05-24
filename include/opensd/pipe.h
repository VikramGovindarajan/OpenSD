//! \file pipe.h

#ifndef OPENSD_PIPE_H
#define OPENSD_PIPE_H

#include "pugixml.hpp"

#include "opensd/node.h"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

class Pipe;

//==============================================================================
//! \class Pipe
//==============================================================================

class Pipe {
public:
  std::string identifier; //!< User-defined identifier
  Node dnode;
  Node unode;
  double diameter; //!< Pipe diameter in [m]
  double length; //!< Pipe length in [m]
  explicit Pipe(pugi::xml_node pipe_node);

protected:

};

//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_PIPE_H
