// Widh Jio
// poscas.cpp

#include <cerrno>

#include "syaa.hpp"

using std::cerr;
using std::dec;
using std::endl;
using std::hash;
using std::hex;
using std::size_t;
using std::string;

// Syaa program flags
bool wannaSkipKeras = false;

// Websocket server configuration
unsigned short port = 11900;
string site = "localhost";
string protocol = "https";

void setPort(unsigned short newPort)
{
  port = newPort;
  $info << "Set ws port to: " << port << endl;
}
void setSite(string newSite)
{
  site = string(newSite);
  $info << "Set ws site to: " << site << endl;
}
void setProtocol(string newProtocol) {
  protocol = string(newProtocol);
  $info << "Set ws protocol to: " << protocol << endl;
}

// Keras configuration
string model = "model";

// At exit or on error
bool isErrorExit = false;

void _terminate()
{
  if (!isErrorExit && errno != 0)
    terminateP("On-exit error detected");
  else if (!isErrorExit)
    $info << "Syaa server is ended without any error! Shutting down..." << endl;
  else
    $warn << "Shutting down the program due to the error" << endl;
}
void terminate(string err, bool callPerror)
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
  exit(errHash);
}
void terminateP(char *err)
{
  string str(err);
  terminate(err, true);
}
void terminate(char *err)
{
  string str(err);
  terminate(str, false);
}
void terminateP(char const *err)
{
  string str(err);
  terminate(str, true);
}
void terminate(char const *err)
{
  string str(err);
  terminate(str, false);
}
void terminateP(string err)
{
  terminate(err, true);
}
void terminate(string err)
{
  terminate(err, false);
}
