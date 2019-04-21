// Widh Jio

#include "ws.hpp"

using std::unique_lock;
using std::mutex;
using std::endl;

using namespace ws;

void onConnected()
{
  unique_lock<mutex> lock(it);
  cond.notify_all();
  connOK = true;
  $info << $ns("ws") << "Socket connected" << endl;
  lock.unlock();
}

void onClose(sio::client::close_reason const& reason)
{
  $info << $ns("ws") << "Socket closed" << reason << ". Retrying..." << endl;
  con2Central();
}

void onFail()
{
  $info << $ns("ws") << "Socket Failure! Retrying..." << endl;
  con2Central();
}

void ws::bindEvents(sio::client* cli) {
  // Bind socket default listeners
  cli->set_open_listener(static_cast<void (*)()>(&onConnected));
  cli->set_close_listener(static_cast<void (*)(const sio::client::close_reason &)>(&onClose));
  cli->set_fail_listener(static_cast<void (*)()>(&onFail));
}
