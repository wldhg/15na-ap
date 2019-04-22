// Widh Jio
// keras.hpp

#include <fdeep/fdeep.hpp>
#include <vector>

#include "syaa.hpp"

#ifndef SYAA_KERAS
#define SYAA_KERAS

namespace keras
{

fdeep::model *loadModel(std::string model);
fdeep::model *loadDummyModel();
void predict(std::vector<double>&);

enum ch
{
  WAKEUP,
  SYNCOPE,
}; // Changes

extern fdeep::model *m;
} // namespace keras

#endif
