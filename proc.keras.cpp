// Widh Jio
// proc.keras.cpp

#include "common.hpp"
#include "keras.hpp"

void load_model() {
  const auto model = fdeep::load_model("fdeep_model.json");
  const auto result = model.predict(
    {fdeep::tensor5(fdeep::shape5(1,1,1,1,4), {1,2,3,4})}
  );
  std::cout << fdeep::show_tensor5s(result) << std::Endl;
}
