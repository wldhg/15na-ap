// Widh Jio
// interface.ws.cpp

#include "ws.hpp"

using sio::socket;
using std::condition_variable_any;
using std::endl;
using std::mutex;
using std::string;
using std::to_string;
using std::unique_lock;

using namespace ws;

mutex ws::it;
condition_variable_any ws::cond;
bool ws::connOK = false;
sio::client ws::cli;
socket::ptr ws::soc;
string ws::url;

void ws::init()
{
  bindListeners(&cli);
  url = protocol + "://" + site + ":" + to_string(port) + "/";
  $info << $ns("ws") << "Trying to connect to " << url << " ..." << endl;
  con2Central();
}

void ws::con2Central()
{
  cli.connect(url.c_str());
  unique_lock<mutex> lock(it);
  if (!connOK)
  {
    cond.wait(lock);
  }
  lock.unlock();
  soc = cli.socket(string("/15na-ws/in"));
  $info << $ns("ws") << "Namespace changed to /15na-ws/in" << endl;
  ws::bindEvents(soc);
  $info << $ns("ws") << "Socket session successfully opened" << endl;
}
