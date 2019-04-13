// Widh Jio
// server.ws.cpp

#include "ws.hpp"

using std::string;
using std::to_string;
using namespace sio;

void ws::openWSChannel() {
  client h;
  h.connect("ws://" + site + ":" + to_string(port));
}
