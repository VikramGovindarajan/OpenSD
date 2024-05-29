//! \file pipe.h

#ifndef OPENSD_PIPE_H
#define OPENSD_PIPE_H

#include "pugixml.hpp"

#include "opensd/node.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================
class Circuit;

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
  Circuit* circuit;
  vector<PFace*> faces;
  double ufrac;
  double dfrac;
  int npar;
  double qcrit;
  double Kforward_old;
  double Kforward;
  double heat_input;
  double mflow;
  double cfarea1;

  
  Pipe(std::string identifier, Circuit* circuit, double diameter, double length, Node* unode, double ufrac, Node* dnode, double dfrac, double ficopt, double roughness, int ncell, double heat_input, double cfarea, int npar, double qcrit, double Kforward, int flowreg);
  // void add_wall(double thk, std::string solname, std::string sollib, int restraint);
  // void update_mflow();
  
  
protected:

};


//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_PIPE_H
