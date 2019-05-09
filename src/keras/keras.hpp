// Widh Jio
// keras.hpp

#include <fdeep/fdeep.hpp>
#include <vector>

#include "15na.hpp"

#ifndef 15na_KERAS
#define 15na_KERAS

namespace keras
{

const fdeep::model *loadModel(std::string model);
const fdeep::model *loadDummyModel();
void predict(std::vector<double> &);

enum ch
{
  WAKEUP,
  SYNCOPE,
}; // Changes

extern const fdeep::model *m;
} // namespace keras

#endif
