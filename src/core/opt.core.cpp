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
  cout << pink << "(Tx) " << def << bold << "15na-ap" << def << " -t [TX_OPTION...]" << endl;
  cout << cyan << "(Rx) " << def << bold << "15na-ap" << def << " AP_ID [RX_OPTION...]" << endl;
  cout << endl;
  cout << pink << "[TX OPTIONS]" << def << endl;
  cout << "  -s=<BYTE>, --size=<BYTE>      Set random packet size in bytes. " << gray << "(Default: 100)" << def << endl;
  cout << "  -d=<US>, --delay=<US>         Set packet emission delay in us. " << gray << "(Default: 0)" << def << endl;
  cout << "  -f=<FLAG>, --flag=<FLAG>      Set tx_monitor_flag. " << gray << "(Default: " << $txflag << ")" << def << endl;
  cout << endl;
  cout << cyan << "[RX OPTIONS]" << def << endl;
  cout << "  -p=<PORT>, --port=<PORT>      Set specific port which is used in socket.io communication. " << gray << "(Default: " << $sioPortString << ")" << def << endl;
  cout << "  -n=<ADDR>, --name=<ADDR>      Set specific" << $sioProtocolName << " server name which is used in socket.io. " << gray << "(Default: localhost)" << def << endl;
  cout << "  -ns=<NS>, --namespace=<NS>    Set socket.io namespace. " << gray << "(Default: 15na-ws/in)" << def << endl;
  cout << "  --debug-packet                Show detailed information for each packets." << endl;
  cout << "  --debug-window                Show detailed process for each windows." << endl;
  cout << endl;
  cout << yellow << "[COMMON OPTIONS]" << def << endl;
  cout << "  -h, --help                    Show this help message." << endl;
  cout << "  -v, --version                 Show 15na AP program version." << endl;
  cout << "  -t, --transmit                Run 15na AP program in transmitter mode." << endl;
  cout << "  -i=<I>, --interface=<I>       Set network interface name. " << gray << "(Default: " << $interface << ")" << def << endl;
  cout << endl;
  cout << "[AP ID]" << endl;
  cout << "  Distinguishable ID string for specify this access point." << endl;
  cout << "  This can be issued by registering a new AP in 15na-serer." << endl;
  cout << endl;
}

void showVersionMessage()
{
  $info << bold << "IRONA AP " << def << gray << "Version " << $version << def << endl;
}

bool core::procFlagOption(string name)
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
  else if (name.compare("transmit") == 0)
  {
    return true;
  }
  else
  {
    terminate("Unknown flag name. Is `=` missed?: " + name);
  }
  return false;
}
bool core::procShortFlagOption(string name)
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
  else if (name.compare("t") == 0)
  {
    return true;
  }
  else
  {
    terminate("Unknown short flag name. Is `=` missed?: " + name);
  }
  return false;
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
  else if (name.compare("interface") == 0)
  {
    setNetworkInterface(value);
  }
  else if (name.compare("flag") == 0)
  {
    setTxFlag(value);
  }
  else if (name.compare("size") == 0)
  {
    setPacketSize((unsigned short)atoi(value.c_str()));
  }
  else if (name.compare("delay") == 0)
  {
    setPacketDelay((unsigned int)atoi(value.c_str()));
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
  else if (name.compare("i") == 0)
  {
    setNetworkInterface(value);
  }
  else if (name.compare("f") == 0)
  {
    setTxFlag(value);
  }
  else if (name.compare("s") == 0)
  {
    setPacketSize((unsigned short)atoi(value.c_str()));
  }
  else if (name.compare("d") == 0)
  {
    setPacketDelay((unsigned int)atoi(value.c_str()));
  }
  else
  {
    terminate("Unknown short data name: " + name);
  }
}

void core::procAPName(string name)
{
  setAPName(name);
}
