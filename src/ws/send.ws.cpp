/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// send.ws.cpp

#include "ws.hpp"
#include "csi.hpp"

using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;
using namespace ws;

void ws::send(unsigned long long len, uint8_t *bytes) {
  string msg = string((const char *)bytes, len);
  soc->emit("neww", sio::binary_message::create(make_shared<string>(msg)));
  $success << $ns("ws") << "An window sent: " << msg.length() << endl;
  free(bytes);
}
