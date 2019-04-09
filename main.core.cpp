// Widh Jio
// main.core.cpp

#include "common.hpp"
#include "core.hpp"

using std::endl;
using std::size_t;
using std::string;

int main(int argc, char **argv)
{

  info << "Initializing " << bold << "POSCA" << def << " server..." << endl;

  // Analyzing arguments
  bool isModelSelected = false;
  for (int i = 0; i < argc; i++)
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
      if (!isModelSelected)
        model = arg;
      else
        terminate("Model name is duplicated: " + model + " & " + arg);
    }
  }

  return 0;
}
