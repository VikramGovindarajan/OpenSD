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
  this->length    = stod(get_node_value(pipe_node, "length"));
  this->dnode_str = get_node_value(pipe_node, "dnode");
  this->unode_str = get_node_value(pipe_node, "unode");
  this->ncell     = 1; //stod(get_node_value(pipe_node, "ncell"));
  this->unode = nullptr;
  this->dnode = nullptr;
  double ufrac;
  double dfrac;
  double cfarea;
  double delx;
  double delz;
  double roughness;
  double fricopt;

}

} // namespace opensd
