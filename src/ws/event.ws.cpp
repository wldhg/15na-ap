/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// event.ws.cpp

#include "ws.hpp"

using std::endl;
using std::mutex;
using std::unique_lock;

using namespace ws;

void onConnected()
{
  unique_lock<mutex> lock(it);
  cond.notify_all();
  connOK = true;
  $info << $ns("ws") << "Socket connected" << endl;
  lock.unlock();
}

void onClose(sio::client::close_reason const &reason)
{
  $info << $ns("ws") << "Socket closed with code " << reason << "." << endl;
  if (reason != sio::client::close_reason::close_reason_normal) {
    $info << $ns("ws") << "Retrying to connect..." << endl;
    con2Central();
  }
}

void onFail()
{
  $info << $ns("ws") << "Socket Failure! Retrying..." << endl;
  con2Central();
}

void ws::bindListeners(sio::client *cli)
{
  // Bind socket default listeners
  cli->set_open_listener(static_cast<void (*)()>(&onConnected));
  cli->set_close_listener(static_cast<void (*)(const sio::client::close_reason &)>(&onClose));
  cli->set_fail_listener(static_cast<void (*)()>(&onFail));
}

void ws::bindEvents(sio::socket::ptr soc)
{
  // Bind "on" events
  soc->on("regResult", [&](sio::event &data) {
    sio::message msg = *data.get_message();
    if (sio::message::flag::flag_string == msg.get_flag()) {
      std::string str = msg.get_string();
      if (str == "true") {
        $success << "This AP is successfully registered to the central server" << endl;
      } else {
        $error << str << endl;
      }
    } else {
      $error << "Register result reception error occured" << endl;
    }
  });
}

void ws::registerAP(sio::socket::ptr soc)
{
  // Emit registration
  // TODO: Authentication based on keypair
  soc->emit("reg", apName);
}
