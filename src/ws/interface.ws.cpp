// Widh Jio
// server.ws.cpp

#include "ws.hpp"

using std::mutex;
using std::condition_variable_any;
using std::to_string;
using std::string;
using std::endl;
using std::unique_lock;
using sio::socket;

using namespace ws;

mutex ws::it;
condition_variable_any ws::cond;
bool ws::connOK = false;
sio::client ws::cli;
socket::ptr ws::soc;
string ws::url;

void ws::init() {
  bindEvents(&cli);
  url = protocol + "://" + site + ":" + to_string(port) + "/syaa-ws/in";
  $info << $ns("ws") << "Trying to connect to " << url << " ..." << endl;
  con2Central();
}

void ws::con2Central()
{
  cli.connect(url.c_str());
  unique_lock<mutex> lock(it);
  if (!connOK) {
    cond.wait(lock);
  }
  lock.unlock();
  soc = cli.socket();
  $info << $ns("ws") << "Socket connection completed!";
}

//void ws::broadcast(void server, keras::ch notiCh) {
//}
