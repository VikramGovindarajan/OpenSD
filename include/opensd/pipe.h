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
  int ncell;

protected:

};

class Face {
public:
  int faceno;
  Node* unode;
  double ufrac;
  double uheight;
  Node* dnode;
  double dfrac;
  double dheight;
  double vflow_old;
  double mflow;
  double velocity;
  double heat_input_old;
  double heat_input;
  std::vector<double> heat_hslab;
  std::vector<double> heat_hslab_old;
  bool choked;
  double presidue;
  double Gcr;
  double pcr;

  virtual void assign_statevar();
};

class PFace : public Face {
public:
  Pipe* pipe;
  double diameter;
  double cfarea;
  double delx;
  double delz;
  double roughness;
  double Re;
  double fricopt;
  double fricfact_old;
  double opening;
  // Circuit* circuit;

  PFace(int faceno, Pipe* pipe, Node* unode, double ufrac, Node* dnode, double dfrac, double diameter, double cfarea, double delx, double delz, double fricopt, double roughness);

  virtual void assign_statevar();
};


//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_PIPE_H
