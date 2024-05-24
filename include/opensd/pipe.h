//! \file pipe.h

#ifndef OPENSD_PIPE_H
#define OPENSD_PIPE_H

#include "pugixml.hpp"
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
  std::string identifier_; //!< User-defined identifier
  std::string dnode_;
  std::string unode_;
  double diameter_; //!< Pipe diameter in [m]
  double length_; //!< Pipe length in [m]
  explicit Pipe(pugi::xml_node pipe_node);

protected:

};

//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_PIPE_H
