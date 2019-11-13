/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// main.core.cpp

#include "core.hpp"

using std::atexit;
using std::endl;
using std::size_t;
using std::string;
using namespace core;

int main(int argc, char **argv)
{
  std::cout.setf(std::ios::unitbuf);

  $info << "Initializing " << bold << "IRONA AP Program" << def << endl;

  atexit(_terminate);

  // Analyzing arguments
  bool isAPNamed = false;
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
        string value = arg.substr(divPosition + 1);
        (isFullArg ? procDataOption : procShortDataOption)(name, value);
      }
    }
    else
    {
      isAPNamed = true;
      procAPName(arg);
    }
  }

  if (isAPNamed) {
    $success << "Checked arguments" << endl;
  } else {
    terminate("No AP ID provided");
  }

  // Start websocket server
  ws::init();

  // Open linux connector socket
  csi::openSocket();

  return 0;
}
