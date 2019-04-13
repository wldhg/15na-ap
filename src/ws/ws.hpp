// Widh Jio
// ws.hpp

#include <server_ws.hpp>

#include "poscas.hpp"
#include "keras.hpp"

#ifndef POSCA_WS
#define POSCA_WS

typedef SimpleWeb::SocketServer<SimpleWeb::WS> WSServer;
typedef std::shared_ptr<WSServer::Connection> Connection;

namespace ws
{

const WSServer& openChannel();
void broadcast(WSServer&, keras::ch);
}

#endif
