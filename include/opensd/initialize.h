#ifndef OPENSD_INITIALIZE_H
#define OPENSD_INITIALIZE_H

#include <string>

#include <opensd/circuit.h>
#include <opensd/vector.h>

namespace opensd {

namespace model {
extern vector<Circuit> circuits;
} // namespace model

// int parse_command_line(int argc, char* argv[]);

//! Read circuit, heat slab, and settings from a single XML file
// bool read_model_xml();
//! Read inputs from separate XML files
void read_separate_xml_files();
//! Write some output that occurs right after initialization
// void initial_output();

void attach_faces();

} // namespace opensd

#endif // OPENSD_INITIALIZE_H
