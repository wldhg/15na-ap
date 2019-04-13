// Widh Jio
// proc.keras.cpp

#include "keras.hpp"

using std::string;
using std::endl;
using namespace fdeep;

fdeep::model *m = NULL;

void keras::loadModel(string model) {
  $info << "Using Keras model named \"" << model << "\"." << endl;
  try {
    *m = load_model(model.c_str());
  } catch (...) {
    terminateP("Error occured while loading model");
  }
}

void keras::loadDummyModel() {

}

void keras::predict() {

}
