//! \file pipe.cpp
#include "opensd/pipe.h"

#include <iostream>

#include "opensd/error.h"
#include "opensd/xml_interface.h"
#include "opensd/vector.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

//==============================================================================
// Pipe implementation
//==============================================================================

Pipe::Pipe(pugi::xml_node pipe_node)
{
  if (check_for_node(pipe_node, "identifier")) {
    this->identifier = get_node_value(pipe_node, "identifier");
  } else {
    fatal_error("Must specify identifier of pipe in geometry XML file.");
  }

  this->diameter  = stod(get_node_value(pipe_node, "diameter"));
  length    = stod(get_node_value(pipe_node, "length"));
  dnode_str = get_node_value(pipe_node, "dnode");
  unode_str = get_node_value(pipe_node, "unode");
  ncell     = 1; //stod(get_node_value(pipe_node, "ncell"));
  Node unode;
  double ufrac;
  Node dnode;
  double dfrac;
  double cfarea;
  double delx;
  double delz;
  double roughness;
  double fricopt;

  vector<Node> nodes;
  for (int i = 0; i < ncell - 1; ++i) {
    Node node = Node();
    // node->identifier = identifier + "_node" + std::to_string(i);
    // node->height = unode->height + (dnode->height - unode->height) * (i + 1) / ncell;
    // nodes.push_back(node);
  }
  vector<PFace> faces;
  for (int i = 0; i < ncell; ++i) {
    if (i == 0 && ncell == 1) {
        faces.push_back(PFace(i, *this, unode, ufrac, dnode, dfrac, diameter, this->cfarea, delx, delz, fricopt, roughness));
    } else if (i == 0) {
        faces.push_back(PFace(i, *this, unode, ufrac, nodes[0], -1, diameter, cfarea, delx, delz, fricopt, roughness));
    } else if (i == ncell-1) {
        faces.push_back(PFace(i, *this, nodes[ncell-2], -1, dnode, dfrac, diameter, cfarea, delx, delz, fricopt, roughness));
    } else {
        faces.push_back(PFace(i, *this, nodes[i-1], -1, nodes[i], -1, diameter, cfarea, delx, delz, fricopt, roughness));
    }
  }
}

PFace::PFace(int faceno, Pipe pipe, Node unode, double ufrac, Node dnode, double dfrac, 
  double diameter, double cfarea, double delx, double delz, double fricopt, double roughness) {
  
}

} // namespace opensd
