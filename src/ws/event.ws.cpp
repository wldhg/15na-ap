// Widh Jio

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
  // NOTE: This part is not used for now, but used later for authentication
}
