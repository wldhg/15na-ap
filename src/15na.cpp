/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// 15na.cpp

#include <cerrno>

#include "15na.hpp"

using std::cerr;
using std::dec;
using std::endl;
using std::hash;
using std::hex;
using std::size_t;
using std::string;

// 15na program settings
bool wannaDebugPacket = false;
bool wannaDebugWindow = false;
string netInterface = $interface;

// Packet injection configuration
string txFlag = $txflag;
unsigned short pktSize = 100;
unsigned int pktDelay = 0;

// Websocket server configuration
unsigned short port = $port;
string site = "localhost";
string ns = "15na-ws/in";
string apName = "";

void setNetworkInterface(string newInterface)
{
  netInterface = string(newInterface);
  $info << "Set network interface to: " << netInterface << endl;
}

void setPort(unsigned short newPort)
{
  port = newPort;
  $info << "Set ws port to: " << port << endl;
}
void setSite(string newSite)
{
  site = string(newSite);
  $info << "Set ws site name to: " << site << endl;
}
void setNamespace(string newNamespace)
{
  ns = string(newNamespace);
  $info << "Set ws namespace to: " << ns << endl;
}
void setAPName(string concatName)
{
  if (apName.length() > 0) {
    apName += " " + concatName;
  } else {
    apName += concatName;
  }
}

void setTxFlag(string newFlag)
{
  txFlag = string(newFlag);
  $info << "Set txFlag to: " << txFlag << endl;
}
void setPacketSize(unsigned short newSize)
{
  pktSize = newSize;
  $info << "Set emitting packet size to: " << pktSize << " bytes" << endl;
}
void setPacketDelay(unsigned int newDelay)
{
  pktDelay = newDelay;
  $info << "Set emission delay to: " << pktDelay << " us" << endl;
}

// At exit or on error
bool isErrorExit = false;

void _terminate()
{
  if (!isErrorExit && errno != 0)
    terminateP("On-exit error detected");
  else if (!isErrorExit)
    $info << "The program is ended without any error! Shutting down..." << endl;
  else
    $warn << "Shutting down the program due to an error" << endl;
}
void terminate(string err, bool callPerror, bool dontExit)
{
  isErrorExit = true;
  size_t errHash = hash<string>{}(err);
  $error << red << "(0x" << hex << errHash << ") " << def << dec;
  if (callPerror)
  {
    perror(err.c_str());
  }
  else
  {
    cerr << err << endl;
  }
  if (!dontExit) {
    exit(errHash);
  }
}
void terminateP(char *err)
{
  string str(err);
  terminate(err, true, false);
}
void terminate(char *err)
{
  string str(err);
  terminate(str, false, false);
}
void terminateP(char const *err)
{
  string str(err);
  terminate(str, true, false);
}
void terminate(char const *err)
{
  string str(err);
  terminate(str, false, false);
}
void terminateP(string err)
{
  terminate(err, true, false);
}
void terminate(string err)
{
  terminate(err, false, false);
}
void error(string err)
{
  terminate(err, false, true);
}
void error(char const* err)
{
  string str(err);
  terminate(str, false, true);
}
void error(char* err)
{
  string str(err);
  terminate(str, false, true);
}
void errorP(string err)
{
  terminate(err, true, true);
}
void errorP(char const* err)
{
  string str(err);
  terminate(str, true, true);
}
void errorP(char* err)
{
  string str(err);
  terminate(str, true, true);
}
