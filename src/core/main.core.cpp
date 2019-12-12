/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// main.core.cpp

#include <unistd.h>

#include "core.hpp"

using std::atexit;
using std::endl;
using std::size_t;
using std::string;
using namespace core;

int main(int argc, char **argv)
{
  std::cout.setf(std::ios::unitbuf);

  // Analyzing arguments
  bool isAPNamed = false;
  bool isTxMode = false;
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
        isTxMode = (isFullArg ? procFlagOption : procShortFlagOption)(arg);
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

  if (isTxMode) {
    $success << "Checked arguments, running in transmitter mode." << endl;
  } else if (isAPNamed) {
    $success << "Checked arguments, running in receiver mode." << endl;
  } else {
    terminate("No AP ID provided or Tx flag not detected!");
  }

  // Initialize network interface
  $info << "Initializing " << bold << "IRONA AP " << (isTxMode ? pink : cyan) << "❤" << def << endl;
  if (geteuid()) {
    terminate("Root privilege is required to bind socket with kernel and inject packets.");
  }
  atexit(_terminate);
  init(isTxMode);

  if (isTxMode) {
    // Start to send packets
    csi::makePacket();
  } else {
    // Start websocket server
    ws::init();

    // Open linux connector socket
    csi::openSocket();
  }

  return 0;
}
