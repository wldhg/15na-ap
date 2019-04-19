// Widh Jio
// keras.hpp

#include <fdeep/fdeep.hpp>

#include "syaa.hpp"

#ifndef SYAA_KERAS
#define SYAA_KERAS

namespace keras
{

fdeep::model *loadModel(std::string model);
fdeep::model *loadDummyModel();
void predict();

enum ch
{
  WAKEUP,
  SYNCOPE,
}; // Changes
} // namespace keras

#endif
