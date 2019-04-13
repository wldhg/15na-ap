// Widh Jio
// common.cpp

#include "common.hpp"

using std::dec;
using std::endl;
using std::hash;
using std::hex;
using std::size_t;
using std::string;

// Websocket server configuration
unsigned short port = 7654;
string site = "localhost";

void setPort(unsigned short newPort)
{
  port = newPort;
  $info << "Set ws port to: " << port << endl;
}
void setSite(string newSite) {
  site = string(newSite);
  $info << "Set ws site to: " << site << endl;
}

// Keras configuration
string model = "model";

// At exit or on error
bool isErrorExit = false;

void _terminate() {
  if (!isErrorExit)
    $info << "POSCA server is ended without any error! Shutting down..." << endl;
  else
    $warn << "Shutting down the program due to the error" << endl;
}
void terminate(char const *err) {
  return terminate((char *) err);
}
void terminate(char *err)
{
  string str(err);
  return terminate(str);
}
void terminate(string err)
{
  isErrorExit = true;
  size_t errHash = hash<string>{}(err);
  $error << "(0x" << hex << errHash << ") " << dec << err << endl;
  exit(errHash);
}
