//! \file bc.h

#ifndef OPENSD_BC_H
#define OPENSD_BC_H

#include "pugixml.hpp"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

class BC;

//==============================================================================
//! \class BC
//==============================================================================

class BC {
public:
  std::string identifier; //!< User-defined identifier
  std::string node_;
  std::string var_;
  double val_; //!< value in [SI]
  explicit BC(pugi::xml_node bc_node);

protected:

};

//==============================================================================
// Non-member functions
//==============================================================================

} // namespace opensd

#endif // OPENSD_PIPE_H
