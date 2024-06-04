#ifndef CONNECTION_H
#define CONNECTION_H

#include "node.h" // Include necessary headers for Node, TPTank, and other dependencies

namespace opensd {
    
class Connection {
  
public:
  Node node;
  double frac, height;
  double drho_dp_consth, tpres_gues, tenth_gues, rhomass_gues, viscosity, cpmass, conductivity;
  double tpres_old, tenth_old, rhomass_old;
  Connection(Node node, double frac, double height);
  void update_gues();
  void update_old();
};

} //namespace opensd

#endif