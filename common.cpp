// Widh Jio
// common.cpp

#include "common.hpp"

using std::dec;
using std::endl;
using std::hash;
using std::hex;
using std::size_t;
using std::string;

unsigned short port = 7654;
string site = "localhost";
string model = "model";

void terminate() {
  info << "POSCA server is ended without any error! Shutting down." << endl;
  exit(0);
}
void terminate(char *err)
{
  string str(err);
  return terminate(str);
}
void terminate(string err)
{
  size_t errHash = hash<string>{}(err);
  error << "(0x" << hex << errHash << ") " << dec << err << endl;
  info << "Shutting down POSCA server due to the error." << endl;
  exit(errHash);
}
