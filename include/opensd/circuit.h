//! \file circuit.h

#ifndef OPENSD_CIRCUIT_H
#define OPENSD_CIRCUIT_H

#include "pugixml.hpp"
#include "opensd/node.h"
#include "opensd/pipe.h"
#include "opensd/bc.h"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

class Circuit;

namespace model {
extern vector<Circuit> circuits;
} // namespace model

//==============================================================================
//! \class Circuit
//==============================================================================

class Circuit {
public:
  std::string identifier; //!< User-defined identifier
  std::string flname_;
  explicit Circuit(pugi::xml_node cir_node);
  double eps_m;
  double mean_flow;
  double eps_h;
  double eps_p;
  vector<Node> nodes;
  vector<Pipe> pipes;
  vector<BC> bcs;
protected:

};

//==============================================================================
// Non-member functions
//==============================================================================

void read_circuits(pugi::xml_node node);

} // namespace opensd

#endif // OPENSD_CIRCUIT_H
