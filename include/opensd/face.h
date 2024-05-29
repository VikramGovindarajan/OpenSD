//! \file face.h

#ifndef OPENSD_FACE_H
#define OPENSD_FACE_H

#include "pugixml.hpp"

#include "opensd/node.h"
#include "opensd/pipe.h"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
//! \class Face
//==============================================================================

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
  double vflow_gues;
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
  double tpres_old;
  double spres_old;
  double ttemp_old;
  double stemp_old;
  double tpres_gues;
  double spres_gues;
  double ttemp_gues;
  double stemp_gues;
  
  Face(int faceno, Node* unode, double ufrac, Node* dnode, double dfrac);
  Face() = default;
  virtual ~Face() = default;
  void assign_statevar();
  void update_gues();

};

class PFace : public Face {
public:
  Pipe& pipe;
  double diameter;
  double cfarea;
  double delx;
  double delz;
  double roughness;
  double Re;
  double fricopt;
  double fricfact_old;
  double fricfact_gues;
  double opening;
  Circuit* circuit;
  
  PFace(int faceno, Pipe& pipe, Node* unode, double ufrac, Node* dnode, double dfrac, 
    double diameter, double cfarea, double delx, double delz, double fricopt, double roughness);
  PFace() = default;
  virtual ~PFace() = default;

  // virtual void assign_statevar();
  double eqn_mom(double x, double time, double delt, bool trans_sim, double alpha_mom);
  void update_abcoef(double time, double delt, double trans_sim, double alpha_mom);
  
};


//==============================================================================
// Non-member functions
//==============================================================================


} // namespace opensd

#endif // OPENSD_Face_H
