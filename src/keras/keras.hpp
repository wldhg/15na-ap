// Widh Jio
// keras.hpp

#include <fdeep/fdeep.hpp>

#include "poscas.hpp"

#ifndef POSCA_KERAS
#define POSCA_KERAS

namespace keras
{

fdeep::model* loadModel(std::string model);
fdeep::model* loadDummyModel();
void predict();

enum ch {
  SITDOWN,
  STANDUP,
  TOGOOD,
  TOBAD,
}; // Changes
}

#endif
