//! \file post.h

#ifndef POST_H
#define POST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <memory>
#include <iomanip>
#include <algorithm>

namespace opensd {

class Calculate {
public:
  static std::vector<Calculate*> registry;

  Calculate(double(*func)(const std::vector<double>&), const std::vector<std::string>& comps);
  std::vector<double> timeSeries;
  double val;
};

void openFile(const std::string& outputFile);
void writeOutput(double time, double delt);
void writeHeader();
void writeValue(double time, double delt);
void updateCalcs(double time, double delt);

}

#endif // POST_H
