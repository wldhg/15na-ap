// Widh Jio
// proc.keras.cpp

#include "keras.hpp"
#include "ws.hpp"

using std::string;
using std::vector;
using std::endl;
using namespace fdeep;

fdeep::model *keras::m = NULL;

fdeep::model* keras::loadModel(string model) {
  $info << "Using Keras model named \"" << model << "\"." << endl;
  try {
    *keras::m = load_model(model.c_str());
  } catch (...) {
    terminateP("Error occured while loading model");
  }
  $success << "Loaded Keras model from " << model << endl;
  return keras::m;
}

fdeep::model* keras::loadDummyModel()
{
  $success << "Loaded Keras dummy model" << endl;
  return NULL;
}

void keras::predict(vector<double>& csis) {
  // Convert to float vector
  const vector<float> fCSIs(csis.begin(), csis.end());
  // Convert to tensor5
  const fdeep::shared_float_vec sv(fplus::make_shared_ref<fdeep::float_vec>(std::move(fCSIs)));
  fdeep::tensor5 t(fdeep::shape5(csis.size(), 1, 0, 0, 0), sv);
  // Predict
  const auto prediction = keras::m->predict({t});
  // Afterprocess
  $debug << $ns("keras") << fdeep::show_tensor5s(prediction) << endl;
  if (false) { // If syncoped
    ws::soc->emit("alert");
  }
  if (false) { // If waked up
    ws::soc->emit("discard");
  }
}
