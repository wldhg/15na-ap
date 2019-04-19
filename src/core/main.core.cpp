// Widh Jio
// main.core.cpp

#include "core.hpp"

using std::endl;
using std::size_t;
using std::string;
using std::atexit;
using namespace core;

int main(int argc, char **argv)
{
  $info << "Initializing " << bold << "Syaa" << def << " server..." << endl;

  atexit(_terminate);

  // Analyzing arguments
  bool isModelSelected = false;
  for (int i = 1; i < argc; i++)
  {
    string arg = c2str(argv[i]);

    if (arg.find("-") == 0)
    {
      // Options
      bool isFullArg = arg.find("--") == 0;
      arg = arg.substr(isFullArg ? 2 : 1);
      size_t divPosition = arg.find("=");
      if (divPosition == string::npos)
      {
        // Flag option
        (isFullArg ? procFlagOption : procShortFlagOption)(arg);
      }
      else
      {
        // Data option
        string name = arg.substr(0, divPosition);
        string value = arg.substr(divPosition);
        (isFullArg ? procDataOption : procShortDataOption)(name, value);
      }
    }
    else
    {
      // Model name
      if (!isModelSelected) {
        model = arg;
        isModelSelected = true;
      } else
        terminate("Model name is duplicated: " + model + " & " + arg);
    }
  }

  if (!isModelSelected)
    terminate("Enter the name of model file");

  $success << "Checked arguments" << endl;

  // Load Keras model
  fdeep::model* m = NULL;
  if (wannaSkipKeras) {
    m = keras::loadDummyModel();
  } else {
    m = keras::loadModel(model);
  }

  // Load CSI logger
  csi::openSocket();

  // Start websocket server
  const WSServer& server = ws::openChannel();

  // Start Sea server
  startSyaa(m, (WSServer*) &server);

  return 0;
}
