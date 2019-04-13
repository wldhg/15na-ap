// Widh Jio
// server.ws.cpp

#include <memory>
#include <thread>
#include <unordered_set>

#include "ws.hpp"

using std::endl;
using std::shared_ptr;
using std::string;
using std::thread;
using std::unordered_set;
using std::to_string;
using namespace std::this_thread;

const WSServer& ws::openChannel()
{
  // Create WS server instance
  WSServer *server = new WSServer();
  server->config.port = port;
  //server.config.address = site;

  // Set endpoint on "/ps"
  auto &ep = server->endpoint["^/ps/?$"];

  ep.on_open = [](Connection con) {
    $info << $ns("ws") << "New connection established: " << gray << con.get() << def << endl;
    $debug << "psConnected/" + string($version) << endl;
    con->send("psConnected/" + string($version), [](const SimpleWeb::error_code &error) {
      if (error.value() != 0)
        $warn << $ns("ws") << gray << "On Connection Notification" << red << " (" << error << ") " << def << error.message() << endl;
    });
  };

  ep.on_close = [](Connection con, int status, const string &reason) {
    // See RFC 6455 7.4.1. for status codes
    $info << $ns("ws") << "Connection broken: " << gray << con.get() << " (" << status << ":" << reason << ")" << def << endl;
  };

  ep.on_handshake = [](Connection con, SimpleWeb::CaseInsensitiveMultimap &resHeader) {
    $info << $ns("ws") << "Incoming handshake: " << gray << con.get() << def << endl;
    return SimpleWeb::StatusCode::information_switching_protocols;
  };

  ep.on_error = [](Connection con, const SimpleWeb::error_code &error) {
    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html for error codes
    $error << $ns("ws") << gray << con.get() << red << " (" << error << ") " << def << error.message() << endl;
  };

  thread thServer([server]() {
    $info << $ns("ws") << "Starting websocket server on ws://" << site << ":" << port << "/ps ..." << endl;
    server->start();
    $info << $ns("ws") << "Websocket server successfully launched." << endl;
  });
  thServer.detach();

  return *server;
}

void ws::broadcast(WSServer& server, keras::ch notiCh) {
  unordered_set<Connection> cons = server.get_connections();
  for (const Connection con : cons) {
    $debug << "psChanged/" + to_string(notiCh) << endl;
    con->send("psChanged/" + to_string(notiCh), [](const SimpleWeb::error_code &error) {
      if (error.value() != 0)
        $warn << $ns("ws") << gray << "On Broadcasting" << red << " (" << error << ") " << def << error.message() << endl;
    });
  }
}
