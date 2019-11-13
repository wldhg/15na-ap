/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// opt.core.cpp

#include "core.hpp"

using std::cout;
using std::endl;
using std::string;

void showHelpMessage()
{
  cout << endl;
  cout << bold << "15na-ap" << def << " [OPTION...]" << endl;
  cout << endl;
  cout << "[OPTIONS]" << endl;
  cout << "  -p=<PORT>, --port=<PORT>      Set specific port which is used in socket.io communication. (Default: " << $sioPortString << ")" << endl;
  cout << "  -n=<ADDR>, --name=<ADDR>      Set specific" << $sioProtocolName << " server name which is used in socket.io. (Default: localhost)" << endl;
  cout << "  -ns=<NS>, --namespace=<NS>    Set socket.io namespace. (Default: 15na-ws/in)" << endl;
  cout << "  -h, --help                    Show this help message." << endl;
  cout << "  -v, --version                 Show 15na AP program version." << endl;
  cout << endl;
  cout << "[DEBUG OPTIONS]" << endl;
  cout << "  --debug-packet                Show detailed information for each packets." << endl;
  cout << "  --debug-window                Show detailed process for each windows." << endl;
  cout << endl;
}

void showVersionMessage()
{
  $info << bold << "IRONA AP Program " << def << gray << "Version " << $version << def << endl;
}

void core::procFlagOption(string name)
{
  if (name.compare("help") == 0)
  {
    showHelpMessage();
    exit(0);
  }
  else if (name.compare("version") == 0)
  {
    showVersionMessage();
    exit(0);
  }
  else if (name.compare("debug-packet") == 0)
  {
    wannaDebugPacket = true;
    $info << "Each received packets will be debugged." << endl;
  }
  else if (name.compare("debug-window") == 0)
  {
    wannaDebugWindow = true;
    $info << "Each windows will be debugged." << endl;
  }
  else
  {
    terminate("Unknown flag name. Is `=` missed?: " + name);
  }
}
void core::procShortFlagOption(string name)
{
  if (name.compare("h") == 0)
  {
    showHelpMessage();
    exit(0);
  }
  else if (name.compare("v") == 0)
  {
    showVersionMessage();
    exit(0);
  }
  else
  {
    terminate("Unknown short flag name. Is `=` missed?: " + name);
  }
}

void core::procDataOption(string name, string value)
{
  if (name.compare("port") == 0)
  {
    setPort((unsigned short)atoi(value.c_str()));
  }
  else if (name.compare("name") == 0)
  {
    setSite(value);
  }
  else if (name.compare("namespace") == 0)
  {
    setNamespace(value);
  }
  else
  {
    terminate("Unknown data name: " + name);
  }
}
void core::procShortDataOption(string name, string value)
{
  if (name.compare("p") == 0)
  {
    setPort((unsigned short)atoi(value.c_str()));
  }
  else if (name.compare("n") == 0)
  {
    setSite(value);
  }
  else if (name.compare("ns") == 0)
  {
    setNamespace(value);
  }
  else
  {
    terminate("Unknown short data name: " + name);
  }
}
