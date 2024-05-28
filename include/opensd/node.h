//! \file node.h

#ifndef OPENSD_NODE_H
#define OPENSD_NODE_H

#include "pugixml.hpp"

// #include "opensd/circuit.h"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

class PFace;
//==============================================================================
//! \class Node
//==============================================================================

class Node {
public:
  std::string identifier; //!< User-defined identifier
  // virtual ~Node() = default;
  double mresidue;
  double msource;
  vector<PFace*> ifaces;
  vector<PFace*> ofaces;
  double mflow_in;
  double mflow_out;
  // Circuit* circuit;
  int node_ind;
  
  double tpres_old;
  double ttemp_old;
  double tenth_old;
  double tpres_gues;
  double ttemp_gues;
  double tenth_gues;
  
  double heat_input;
  double elevation;
  
  vector<std::string> fixed_var;
  double esource;
  double hresidue;
  double volume;
  

  explicit Node(pugi::xml_node flnode_node);
  Node(std::string identifier, double volume, double heat_input, double elevation, double tpres_old = 0.0, double ttemp_old = 0.0, double tenth_old = 0.0); //Circuit* circuit, 
  Node() = default;

  double eqn_cont(double alpha_mom);
  void update_gues();
};

//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_NODE_H
