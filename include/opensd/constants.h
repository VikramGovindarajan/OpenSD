//! \file constants.h
//! A collection of constants

#ifndef OPENSD_CONSTANTS_H
#define OPENSD_CONSTANTS_H


namespace opensd {

constexpr double PI {3.141592653589793238462643383279502884L};

enum class RunMode {
  UNSET, // default value, OpenSD throws error if left to this
  STEADY,
  DESIGN,
  SENSITIVITY,
  OPTIMIZE,
  TRANSIENT
};


} // namespace opensd

#endif // OPENSD_CONSTANTS_H
