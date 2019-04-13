// Widh Jio
// proc.keras.cpp

#include "keras.hpp"

using std::string;
using std::endl;
using namespace fdeep;

fdeep::model* keras::loadModel(string model) {
  $info << "Using Keras model named \"" << model << "\"." << endl;
  fdeep::model *m = NULL;
  try {
    *m = load_model(model.c_str());
  } catch (...) {
    terminateP("Error occured while loading model");
  }
  $success << "Loaded Keras model from " << model << endl;
  return m;
}

fdeep::model* keras::loadDummyModel()
{
  $success << "Loaded Keras dummy model" << endl;
  return NULL;
}

void keras::predict() {

}
